#ifndef PreConditionPlan_H_
#define PreConditionPlan_H_

#include "DomainCondition.h"
#include "engine/BasicUtilityFunction.h"
#include "engine/DefaultUtilityFunction.h"
#include "engine/UtilityFunction.h"
/*PROTECTED REGION ID(incl1418042796751) ENABLED START*/
// Add inlcudes here
/*PROTECTED REGION END*/
using namespace alica;

namespace alicaAutogenerated
{
/*PROTECTED REGION ID(meth1418042796751) ENABLED START*/
// Add other things here
/*PROTECTED REGION END*/
class UtilityFunction1418042796751 : public BasicUtilityFunction
{
    shared_ptr<UtilityFunction> getUtilityFunction(Plan* plan);
};

class PreCondition1418042929966 : public DomainCondition
{
    bool evaluate(shared_ptr<RunningPlan> rp);
};

} // namespace alicaAutogenerated

#endif
