
#include "engine/AlicaContext.h"
#include "engine/AlicaEngine.h"

namespace alica
{

constexpr int ALICA_VERSION_MAJOR = 1;
constexpr int ALICA_VERSION_MINOR = 0;
constexpr int ALICA_VERSION_PATCH = 0;
constexpr int ALICA_VERSION = (ALICA_VERSION_MAJOR * 10000) + (ALICA_VERSION_MINOR * 100) + ALICA_VERSION_PATCH;
constexpr int ALICA_CTX_GOOD = 0xfaadfaad;
constexpr int ALICA_CTX_BAD = 0xdeaddead;

AlicaContext::AlicaContext(const std::string& roleSetName, const std::string& masterPlanName, bool stepEngine)
        : _validTag(ALICA_CTX_GOOD)
        , _engine(std::make_unique<AlicaEngine>(*this, roleSetName, masterPlanName, stepEngine))
{
}

AlicaContext::~AlicaContext()
{
    _validTag = ALICA_CTX_BAD;
}

int AlicaContext::init(AlicaCreators& creatorCtx)
{
    if (_communicator) {
        _communicator->startCommunication();
    }

    return _engine->init(creatorCtx);
}

int AlicaContext::terminate()
{
    if (_communicator) {
        _communicator->stopCommunication();
    }
    _engine->shutdown();
    essentials::SystemConfig::getInstance().shutdown();
    // TODO: Fix this (add proper return code in engine shutdown)
    return 0;
}

std::string AlicaContext::getRobotName() const
{
    return essentials::SystemConfig::getInstance().getHostname();
}

void AlicaContext::setRobotName(const std::string& name)
{
    essentials::SystemConfig::getInstance().setHostname(name);
}

void AlicaContext::setRootPath(const std::string& path)
{
    essentials::SystemConfig::getInstance().setRootPath(path);
}

void AlicaContext::setConfigPath(const std::string& path)
{
    essentials::SystemConfig::getInstance().setConfigPath(path);
}

bool AlicaContext::isValid()
{
    return _validTag == ALICA_CTX_GOOD;
}
}