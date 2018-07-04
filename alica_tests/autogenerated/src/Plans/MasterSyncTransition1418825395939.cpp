#include "Plans/MasterSyncTransition1418825395939.h"
using namespace alica;
/*PROTECTED REGION ID(eph1418825395939) ENABLED START*/ // Add additional using directives here
#include "TestWorldModel.h"
#include <engine/AgentIDConstPtr.h>
#include <engine/AlicaEngine.h>
/*PROTECTED REGION END*/
namespace alicaAutogenerated
{
// Plan:MasterSyncTransition

/* generated comment

 Task: AttackTask  -> EntryPoint-ID: 1418825395941

 Task: DefaultTask  -> EntryPoint-ID: 1418825402617

 */
shared_ptr<UtilityFunction> UtilityFunction1418825395939::getUtilityFunction(Plan* plan)
{
    /*PROTECTED REGION ID(1418825395939) ENABLED START*/

    shared_ptr<UtilityFunction> defaultFunction = make_shared<DefaultUtilityFunction>(plan);
    return defaultFunction;

    /*PROTECTED REGION END*/
}

// State: FirstTaskFirstState in Plan: MasterSyncTransition

/*
 *
 * Transition:
 *   - Name: MISSING_NAME, ConditionString: , Comment :
 *
 * Plans in State:
 *
 * Tasks:
 *   - AttackTask (1407153522080) (Entrypoint: 1418825395941)
 *   - DefaultTask (1225112227903) (Entrypoint: 1418825402617)
 *
 * States:
 *   - FirstTaskFirstState (1418825395940)
 *   - SecondTaskFirstState (1418825404963)
 *   - FirstTaskSecondState (1418825409988)
 *   - SecondTaskSecondState (1418825411686)
 *
 * Vars:
 */
bool TransitionCondition1418825427317::evaluate(shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1418825425833) ENABLED START*/
    int id8 = 8;
    AgentIDConstPtr agentID8 = rp->getAlicaEngine()->getID<int>(id8);

    if (*(rp->getOwnID()) == *agentID8) {
        return alicaTests::TestWorldModel::getOne()->isTransitionCondition1418825427317();
    } else {
        return alicaTests::TestWorldModel::getTwo()->isTransitionCondition1418825427317();
    }

    /*PROTECTED REGION END*/
}

// State: SecondTaskFirstState in Plan: MasterSyncTransition

/*
 *
 * Transition:
 *   - Name: MISSING_NAME, ConditionString: , Comment :
 *
 * Plans in State:
 *
 * Tasks:
 *   - AttackTask (1407153522080) (Entrypoint: 1418825395941)
 *   - DefaultTask (1225112227903) (Entrypoint: 1418825402617)
 *
 * States:
 *   - FirstTaskFirstState (1418825395940)
 *   - SecondTaskFirstState (1418825404963)
 *   - FirstTaskSecondState (1418825409988)
 *   - SecondTaskSecondState (1418825411686)
 *
 * Vars:
 */
bool TransitionCondition1418825428924::evaluate(shared_ptr<RunningPlan> rp)
{
    /*PROTECTED REGION ID(1418825427469) ENABLED START*/
    int id8 = 8;
    AgentIDConstPtr agentID8 = rp->getAlicaEngine()->getID<int>(id8);

    if (*(rp->getOwnID()) == *agentID8) {
        return alicaTests::TestWorldModel::getOne()->isTransitionCondition1418825428924();
    } else {
        return alicaTests::TestWorldModel::getTwo()->isTransitionCondition1418825428924();
    }

    /*PROTECTED REGION END*/
}

// State: FirstTaskSecondState in Plan: MasterSyncTransition

// State: SecondTaskSecondState in Plan: MasterSyncTransition

} // namespace alicaAutogenerated
