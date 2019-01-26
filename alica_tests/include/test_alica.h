#pragma once

#include "BehaviourCreator.h"
#include "ConditionCreator.h"
#include "ConstraintCreator.h"
#include "ConstraintTestPlanDummySolver.h"
#include "UtilityFunctionCreator.h"

#include <communication/AlicaDummyCommunication.h>
#include <engine/AlicaContext.h>
#include <engine/AlicaEngine.h>

#include <csetjmp>
#include <csignal>
#include <string>

#include <ros/ros.h>

#include <gtest/gtest.h>

#define ASSERT_NO_SIGNAL ASSERT_EQ(setjmp(restore_point), 0);

namespace alica
{

// Don't want to expose engine to app developers
struct AlicaTestEngineGetter
{
    static alica::AlicaEngine* getEngine(alica::AlicaContext* ac) { return ac->_engine.get(); }
};

class AlicaTestFixtureBase : public ::testing::Test
{
protected:
    alica::AlicaContext* ac;
    alica::AlicaEngine* ae;
};

class AlicaTestFixture : public AlicaTestFixtureBase
{
protected:
    virtual const char* getRoleSetName() const { return "Roleset"; }
    virtual const char* getMasterPlanName() const = 0;
    virtual bool stepEngine() const { return true; }
    void SetUp() override
    {
        // determine the path to the test config
        ros::NodeHandle nh;
        std::string path;
        nh.param<std::string>("/rootPath", path, ".");
        alica::AlicaContext::setRobotName("nase");
        alica::AlicaContext::setRootPath(path);
        alica::AlicaContext::setConfigPath(path + "/etc");
        ac = new alica::AlicaContext(getRoleSetName(), getMasterPlanName(), stepEngine());
        ASSERT_TRUE(ac->isValid());
        ac->setCommunicator<alicaDummyProxy::AlicaDummyCommunication>();

        alica::AlicaCreators creators(std::make_unique<alica::ConditionCreator>(), std::make_unique<alica::UtilityFunctionCreator>(),
                std::make_unique<alica::ConstraintCreator>(), std::make_unique<alica::BehaviourCreator>());
        EXPECT_TRUE(ac->init(creators));
        ae = AlicaTestEngineGetter::getEngine(ac);
    }

    void TearDown() override
    {
        ac->terminate();
        delete ac;
    }
};

class AlicaTestFixtureWithSolvers : public AlicaTestFixture
{
protected:
    void SetUp() override
    {
        AlicaTestFixture::SetUp();
        ac->addSolver<alica::reasoner::ConstraintTestPlanDummySolver>();
    }
    void TearDown() override { AlicaTestFixture::TearDown(); }
};

class AlicaTestMultiAgentFixtureBase : public ::testing::Test
{
protected:
    std::vector<alica::AlicaContext*> acs;
    std::vector<alica::AlicaEngine*> aes;
};

class AlicaTestMultiAgentFixture : public AlicaTestMultiAgentFixtureBase
{
protected:
    virtual const char* getRoleSetName() const { return "Roleset"; }
    virtual const char* getMasterPlanName() const = 0;
    virtual int getAgentCount() const = 0;
    virtual bool stepEngine() const { return true; }
    virtual const char* getHostName(int agentNumber) const { return "nase"; }
    virtual void SetUp() override
    {
        // determine the path to the test config
        ros::NodeHandle nh;
        std::string path;
        nh.param<std::string>("/rootPath", path, ".");
        alica::AlicaContext::setRootPath(path);
        alica::AlicaContext::setConfigPath(path + "/etc");
        alica::AlicaCreators creators(std::make_unique<alica::ConditionCreator>(), std::make_unique<alica::UtilityFunctionCreator>(),
                std::make_unique<alica::ConstraintCreator>(), std::make_unique<alica::BehaviourCreator>());

        for (int i = 0; i < getAgentCount(); ++i) {
            alica::AlicaContext::setRobotName(getHostName(i));
            alica::AlicaContext* ac = new alica::AlicaContext(getRoleSetName(), getMasterPlanName(), stepEngine());
            ASSERT_TRUE(ac->isValid());
            ac->setCommunicator<alicaDummyProxy::AlicaDummyCommunication>();
            EXPECT_TRUE(ac->init(creators));
            acs.push_back(ac);
            aes.push_back(AlicaTestEngineGetter::getEngine(ac));
        }
    }

    void TearDown() override
    {
        for (alica::AlicaContext* ac : acs) {
            ac->terminate();
            delete ac;
        }
    }
};
}

extern std::jmp_buf restore_point;
void signalHandler(int signal);
void step(alica::AlicaEngine* ae);
