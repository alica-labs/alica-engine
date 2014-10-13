#include <gtest/gtest.h>
#include <engine/AlicaEngine.h>
#include <engine/IAlicaClock.h>
#include "engine/IAlicaCommunication.h"
#include "TestBehaviourCreator.h"
#include "engine/model/Behaviour.h"
#include "engine/PlanRepository.h"
#include <clock/AlicaROSClock.h>
#include <communication/AlicaRosCommunication.h>
#include  "engine/DefaultUtilityFunction.h"
#include "engine/model/Plan.h"
#include "TestConditionCreator.h"
#include "TestConstraintCreator.h"
#include "TestUtilityFunctionCreator.h"

class AlicaEngineTestBehPool : public ::testing::Test
{
protected:
	supplementary::SystemConfig* sc;
	alica::AlicaEngine* ae;
	alica::TestBehaviourCreator* bc;
	alica::TestConditionCreator* cc;
	alica::TestUtilityFunctionCreator* uc;
	alica::TestConstraintCreator* crc;

	virtual void SetUp()
	{
		// determine the path to the test config
		string path = supplementary::FileSystem::getSelfPath();
		int place = path.rfind("devel");
		path = path.substr(0, place);
		path = path + "src/alica/alica_test/src/test";

		// bring up the SystemConfig with the corresponding path
		sc = supplementary::SystemConfig::getInstance();
		sc->setRootPath(path);
		sc->setConfigPath(path + "/etc");

		// setup the engine
		ae = new alica::AlicaEngine();
		bc = new alica::TestBehaviourCreator();
		cc = new alica::TestConditionCreator();
		uc = new alica::TestUtilityFunctionCreator();
		crc = new alica::TestConstraintCreator();
		ae->setIAlicaClock(new alicaRosProxy::AlicaROSClock());
		ae->setCommunicator(new alicaRosProxy::AlicaRosCommunication(ae));
	}

	virtual void TearDown()
	{
		ae->shutdown();
		sc->shutdown();
		delete ae->getCommunicator();
		delete ae->getIAlicaClock();
		delete cc;
		delete bc;
		delete uc;
		delete crc;
	}
};
/**
 * Tests the initialisation of the behaviourPool
 */
TEST_F(AlicaEngineTestBehPool, behaviourPoolInit)
{
	EXPECT_TRUE(ae->init(bc, cc, uc, crc, "Roleset", "MasterPlan", ".", false))
			<< "Unable to initialise the Alica Engine!";
	auto behaviours = ae->getPlanRepository()->getBehaviours();
	alica::IBehaviourPool* bp = ae->getBehaviourPool();
	for (auto behaviourPair : behaviours)
	{
		cout << "Behaviour: " << behaviourPair.second->getName() << endl;
	}

}

