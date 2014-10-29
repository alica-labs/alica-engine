#ifndef MultiAgentTestPlan_H_
#define MultiAgentTestPlan_H_

#include "DomainCondition.h"
#include "engine/BasicUtilityFunction.h"
#include "engine/UtilityFunction.h"
#include "engine/DefaultUtilityFunction.h"
/*PROTECTED REGION ID(incl1413200862180) ENABLED START*/
//Add inlcudes here
/*PROTECTED REGION END*/
using namespace alica;

namespace alicaAutogenerated
{
/*PROTECTED REGION ID(meth1413200862180) ENABLED START*/
//Add other things here
/*PROTECTED REGION END*/
class UtilityFunction1413200862180 : public BasicUtilityFunction
{
  shared_ptr<UtilityFunction> getUtilityFunction(Plan* plan);
};

class TransitionCondition1413201370590 : public DomainCondition
{
  bool evaluate(shared_ptr<RunningPlan> rp);
};

class TransitionCondition1413201052549 : public DomainCondition
{
  bool evaluate(shared_ptr<RunningPlan> rp);
};

class TransitionCondition1413201367990 : public DomainCondition
{
  bool evaluate(shared_ptr<RunningPlan> rp);
};

} /* namespace alica */

#endif
