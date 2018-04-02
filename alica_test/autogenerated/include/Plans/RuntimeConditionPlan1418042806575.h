#ifndef RuntimeConditionPlan_H_
#define RuntimeConditionPlan_H_

#include "DomainCondition.h"
#include "engine/BasicUtilityFunction.h"
#include "engine/UtilityFunction.h"
#include "engine/DefaultUtilityFunction.h"
/*PROTECTED REGION ID(incl1418042806575) ENABLED START*/
// Add inlcudes here
/*PROTECTED REGION END*/
using namespace alica;

namespace alicaAutogenerated {
/*PROTECTED REGION ID(meth1418042806575) ENABLED START*/
// Add other things here
/*PROTECTED REGION END*/
class UtilityFunction1418042806575 : public BasicUtilityFunction {
    shared_ptr<UtilityFunction> getUtilityFunction(Plan* plan);
};

class RunTimeCondition1418042967134 : public DomainCondition {
    bool evaluate(shared_ptr<RunningPlan> rp);
};

}  // namespace alicaAutogenerated

#endif
