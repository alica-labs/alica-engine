#include "engine/teammanager/TeamManager.h"

#include "engine/AlicaEngine.h"
#include "engine/IRoleAssignment.h"
#include "engine/Logger.h"
#include "engine/collections/RobotProperties.h"
#include "engine/containers/AgentQuery.h"
#include "essentials/AgentIDFactory.h"
#include <alica_common_config/debug_output.h>

#include <SystemConfig.h>
#include <iostream>
#include <utility>

namespace
{
thread_local std::shared_ptr<alica::AgentsCache::AgentMap> s_agents;
}

namespace alica
{

AgentsCache::AgentsCache()
        : _agents(std::make_shared<AgentMap>())
{
}

AgentsCache::~AgentsCache()
{
    for (auto& agentEntry : *_agents) {
        delete agentEntry.second;
    }
}

const std::shared_ptr<AgentsCache::AgentMap>& AgentsCache::get() const
{
    if (s_agents != _agents) {
        std::lock_guard<std::mutex> guard(_agentsMutex);
        s_agents = _agents;
    }
    return s_agents;
}

void AgentsCache::addAgents(std::vector<Agent*>& agents)
{
    if (agents.empty()) {
        return;
    }
    // Mutate agent cache and add new agent
    std::lock_guard<std::mutex> guard(_agentsMutex);
    std::shared_ptr<AgentMap> newAgentsMap = std::make_shared<AgentMap>(*_agents);
    for (Agent* agent : agents) {
        auto ret = newAgentsMap->emplace(agent->getId(), agent);
        if (!ret.second) {
            // already existed
            delete agent;
        }
    }
    _agents = std::move(newAgentsMap);
}

TeamManager::TeamManager(AlicaEngine* engine)
        : _localAgent(nullptr)
        , _engine(engine)
        , _agentAnnouncementTimeInterval(AlicaTime::zero())
        , _timeLastAnnouncement(AlicaTime::zero())
{
    essentials::SystemConfig& sc = essentials::SystemConfig::getInstance();
    _teamTimeOut = AlicaTime::milliseconds(sc["Alica"]->get<unsigned long>("Alica.TeamTimeOut", NULL));
    _useAutoDiscovery = sc["Alica"]->get<bool>("Alica.AutoDiscovery", NULL);
    if (_useAutoDiscovery) {
        _agentAnnouncementTimeInterval = AlicaTime::seconds(sc["Alica"]->get<unsigned long>("Alica.AgentAnnouncementTimeInterval", NULL));
        _announcementRetries = sc["Alica"]->get<int>("Alica.AnnouncementRetries", NULL);
    }
    readSelfFromConfig();
    constructAnnouncementMessage();
}

void TeamManager::constructAnnouncementMessage()
{
    _presenceMessage.senderID = _localAgent->getId();
    _presenceMessage.senderName = _localAgent->getName();
    _presenceMessage.senderSdk = _localAgent->getSdk();
    _presenceMessage.planHash = _localAgent->getPlanHash();
    _presenceMessage.role = _localAgent->getProperties().getDefaultRole();
    for (const auto& cap : _localAgent->getProperties().getCharacteristics()) {
        const std::string& name = cap.first;
        const std::string& value = cap.second->getCapValue()->getName();
        _presenceMessage.capabilities.push_back(std::make_pair(name, value));
    }
}

TeamManager::~TeamManager() {}

void TeamManager::setTeamTimeout(AlicaTime t)
{
    AgentsCache::AgentMap& agents = *_agentsCache.get();
    _teamTimeOut = t;

    for (auto& agentEntry : agents) {
        agentEntry.second->setTimeout(t);
    }
}

void TeamManager::readSelfFromConfig()
{
    essentials::SystemConfig& sc = essentials::SystemConfig::getInstance();
    std::string localAgentName = _engine->getRobotName();
    int id = sc["Local"]->tryGet<int>(-1, "Local", localAgentName.c_str(), "ID", NULL);

    AgentAnnouncement aa;
    if (id != -1) {
        aa.senderID = _engine->getId(id);
    } else {
        aa.senderID = _engine->generateId(8);
        ALICA_DEBUG_MSG("tm: Auto generated id " << aa.senderID);
    }

    aa.senderName = localAgentName;
    aa.role = sc["Local"]->get<std::string>("Local", localAgentName.c_str(), "DefaultRole", NULL);
    // TODO: add plan hash
    aa.planHash = 0;
    aa.senderSdk = _engine->getVersion();
    std::shared_ptr<std::vector<std::string>> properties = sc["Local"]->getNames("Local", localAgentName.c_str(), NULL);
    for (const std::string& s : *properties) {
        if (s == "ID" || s == "DefaultRole") {
            continue;
        }

        std::string svalue = sc["Local"]->get<std::string>("Local", localAgentName.c_str(), s.c_str(), NULL);
        aa.capabilities.emplace_back(s, svalue);
    }

    _localAgent = new Agent(_engine, _teamTimeOut, aa);
    _localAgent->setLocal(true);
    std::vector<Agent*> agents;
    agents.push_back(_localAgent);
    _agentsCache.addAgents(agents);
}

ActiveAgentIdView TeamManager::getActiveAgentIds() const
{
    return ActiveAgentIdView(_agentsCache.get());
}

ActiveAgentView TeamManager::getActiveAgents() const
{
    return ActiveAgentView(_agentsCache.get());
}

int TeamManager::getTeamSize() const
{
    AgentsCache::AgentMap& agents = *_agentsCache.get();
    int teamSize = 0;
    for (auto& agentEntry : agents) {
        if (agentEntry.second->isActive()) {
            ++teamSize;
        }
    }
    return teamSize;
}

const Agent* TeamManager::getAgentByID(AgentIDConstPtr agentId) const
{
    return getAgent(agentId);
}

Agent* TeamManager::getAgent(AgentIDConstPtr agentId) const
{
    AgentsCache::AgentMap& agents = *_agentsCache.get();
    auto agentEntry = agents.find(agentId);
    if (agentEntry != agents.end()) {
        return agentEntry->second;
    }

    return nullptr;
}

AgentIDConstPtr TeamManager::getLocalAgentID() const
{
    return _localAgent->getId();
}

void TeamManager::setTimeLastMsgReceived(AgentIDConstPtr agentId, AlicaTime timeLastMsgReceived)
{
    Agent* agent = getAgent(agentId);
    if (agent) {
        agent->setTimeLastMsgReceived(timeLastMsgReceived);
    }
}

bool TeamManager::isAgentActive(AgentIDConstPtr agentId) const
{
    Agent* agent = getAgent(agentId);
    if (agent) {
        return agent->isActive();
    }
    return false;
}

/**
 * Checks if an agent is ignored
 * @param agentId an essentials::AgentID identifying the agent
 */
bool TeamManager::isAgentIgnored(AgentIDConstPtr agentId) const
{
    Agent* agent = getAgent(agentId);
    if (agent) {
        return agent->isIgnored();
    }
    // Ignore agents that are not announced yet
    return true;
}

void TeamManager::setAgentIgnored(AgentIDConstPtr agentId, const bool ignored) const
{
    Agent* agent = getAgent(agentId);
    if (agent) {
        agent->setIgnored(ignored);
    }
}

bool TeamManager::setSuccess(AgentIDConstPtr agentId, const AbstractPlan* plan, const EntryPoint* entryPoint)
{
    Agent* agent = getAgent(agentId);
    if (agent) {
        agent->setSuccess(plan, entryPoint);
        return true;
    }
    return false;
}

bool TeamManager::setSuccessMarks(AgentIDConstPtr agentId, const IdGrp& suceededEps)
{
    Agent* agent = getAgent(agentId);
    if (agent) {
        agent->setSuccessMarks(suceededEps);
        return true;
    }
    return false;
}

const DomainVariable* TeamManager::getDomainVariable(AgentIDConstPtr agentId, const std::string& sort) const
{
    Agent* agent = getAgent(agentId);
    if (agent) {
        return agent->getDomainVariable(sort);
    }
    return nullptr;
}

AgentGrp TeamManager::updateAgents(bool& changedSomeAgent)
{
    AgentsCache::AgentMap& agents = *_agentsCache.get();
    changedSomeAgent = false;
    AgentGrp deactivatedAgentIds;
    for (const auto& agent : agents) {
        bool changedCurrentAgent = agent.second->update();

        if (changedCurrentAgent && !agent.second->isActive()) {
            deactivatedAgentIds.push_back(agent.second->getId());
        }
        changedSomeAgent |= changedCurrentAgent;
    }
    return deactivatedAgentIds;
}

void TeamManager::handleAgentQuery(const AgentQuery& aq) const
{
    if (!_useAutoDiscovery || aq.senderID == _localAgent->getId()) {
        return;
    }

    // TODO: Add sdk compatibility check with comparing major version numbers
    if (aq.senderSdk != _localAgent->getSdk() || aq.planHash != _localAgent->getPlanHash()) {
        ALICA_DEBUG_MSG("tm: Version mismatch ignoring: " << aq.senderID);
        return;
    }

    ALICA_DEBUG_MSG("tm: Responding to agent: " << aq.senderID);
    announcePresence();
}

void TeamManager::handleAgentAnnouncement(const AgentAnnouncement& aa)
{
    if (!_useAutoDiscovery || aa.senderID == _localAgent->getId()) {
        return;
    }

    // TODO: Add sdk compatibility check with comparing major version numbers
    if (aa.senderSdk != _localAgent->getSdk() || aa.planHash != _localAgent->getPlanHash()) {
        ALICA_DEBUG_MSG("tm: Version mismatch ignoring: " << aa.senderID);
        return;
    }

    // Check if agent already exists
    Agent* agentInfo = getAgent(aa.senderID);
    if (agentInfo) {
        return;
    }

    agentInfo = new Agent(_engine, _teamTimeOut, aa);
    agentInfo->setTimeLastMsgReceived(_engine->getAlicaClock().now());
    _engine->editLog().eventOccurred("New Agent(", aa.senderID, ")");
    std::lock_guard<std::mutex> lg(_msgQueueMutex);
    _msgQueue.push_back(agentInfo);
}

void TeamManager::init()
{
    if (_useAutoDiscovery) {
        _timeLastAnnouncement = _engine->getAlicaClock().now();
        announcePresence();
        queryPresence();
    }
}

void TeamManager::announcePresence() const
{
    ALICA_DEBUG_MSG("tm: Announcing presence");
    for (int i = 0; i < _announcementRetries; ++i) {
        _engine->getCommunicator().sendAgentAnnouncement(_presenceMessage);
    }
}

void TeamManager::queryPresence() const
{
    AgentQuery pqMessage;
    pqMessage.senderID = _localAgent->getId();
    pqMessage.senderSdk = _localAgent->getSdk();
    pqMessage.planHash = _localAgent->getPlanHash();
    for (int i = 0; i < _announcementRetries; ++i) {
        _engine->getCommunicator().sendAgentQuery(pqMessage);
    }
}

void TeamManager::tick()
{
    if (!_useAutoDiscovery) {
        return;
    }

    if (!_msgQueue.empty()) {
        std::lock_guard<std::mutex> lg(_msgQueueMutex);
        _agentsCache.addAgents(_msgQueue);
        _msgQueue.clear();
    }
    // check whether its time for new announcement
    AlicaTime now = _engine->getAlicaClock().now();
    if (_timeLastAnnouncement + _agentAnnouncementTimeInterval <= now) {
        _timeLastAnnouncement = now;
        announcePresence();
    }
}
} /* namespace alica */
