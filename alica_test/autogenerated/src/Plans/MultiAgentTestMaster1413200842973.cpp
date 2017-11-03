#include "Plans/MultiAgentTestMaster1413200842973.h"
using namespace alica;
/*PROTECTED REGION ID(eph1413200842973) ENABLED START*/ //Add additional using directives here
#include "TestWorldModel.h"
#include "supplementary/IAgentID.h"
#include <engine/AlicaEngine.h>
/*PROTECTED REGION END*/
namespace alicaAutogenerated
{
//Plan:MultiAgentTestMaster

/* generated comment
 
 Task: DefaultTask  -> EntryPoint-ID: 1413200842975

 */
shared_ptr<UtilityFunction> UtilityFunction1413200842973::getUtilityFunction(Plan* plan)
{
  /*PROTECTED REGION ID(1413200842973) ENABLED START*/

  shared_ptr < UtilityFunction > defaultFunction = make_shared < DefaultUtilityFunction > (plan);
  return defaultFunction;

  /*PROTECTED REGION END*/
}

//State: Init in Plan: MultiAgentTestMaster

/*
 *		
 * Transition:
 *   - Name: , ConditionString: , Comment :  
 *
 * Plans in State: 
 *
 * Tasks: 
 *   - DefaultTask (1225112227903) (Entrypoint: 1413200842975)
 *
 * States:
 *   - Init (1413200842974)
 *   - Start (1413201213955)
 *   - Finished (1413201380359)
 *
 * Vars:
 */
bool TransitionCondition1413201227586::evaluate(shared_ptr<RunningPlan> rp)
{
  /*PROTECTED REGION ID(1413201226246) ENABLED START*/
	int id8 = 8;
	const supplementary::IAgentID * agentID8 =  rp->getAlicaEngine()->getID<int>(id8);

	if(*(rp->getOwnID()) == *agentID8)
	{
		return alicaTests::TestWorldModel::getOne()->isTransitionCondition1413201227586();
	}
	else
	{
		return alicaTests::TestWorldModel::getTwo()->isTransitionCondition1413201227586();
	}
  /*PROTECTED REGION END*/

}

//State: Start in Plan: MultiAgentTestMaster

/*
 *		
 * Transition:
 *   - Name: , ConditionString: , Comment :  
 *
 * Plans in State: 				
 *   - Plan - (Name): MultiAgentTestPlan, (PlanID): 1413200862180 
 *
 * Tasks: 
 *   - DefaultTask (1225112227903) (Entrypoint: 1413200842975)
 *
 * States:
 *   - Init (1413200842974)
 *   - Start (1413201213955)
 *   - Finished (1413201380359)
 *
 * Vars:
 */
bool TransitionCondition1413201389955::evaluate(shared_ptr<RunningPlan> rp)
{
  /*PROTECTED REGION ID(1413201388722) ENABLED START*/
	int id8 = 8;
	const supplementary::IAgentID * agentID8 =  rp->getAlicaEngine()->getID<int>(id8);
	if(*(rp->getOwnID()) == *agentID8)
	{
		return alicaTests::TestWorldModel::getOne()->isTransitionCondition1413201389955() /*&& rp->allChildrenStatus(PlanStatus::Success)*/;
	}
	else
	{
		return alicaTests::TestWorldModel::getTwo()->isTransitionCondition1413201389955() /*&& rp->allChildrenStatus(PlanStatus::Success)*/;
	}
  /*PROTECTED REGION END*/

}

//State: Finished in Plan: MultiAgentTestMaster

}
