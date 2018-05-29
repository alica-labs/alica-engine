#pragma once
#include "engine/Types.h"

#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <unordered_set>

namespace alica
{

class AlicaEngine;

/**
 * Globally holds information about succeeded entrypoints for a specific robot
 */
class SuccessMarks
{
  public:
    SuccessMarks(const AlicaEngine* ae);
    SuccessMarks(const AlicaEngine* ae, const IdGrp& epIds);
    virtual ~SuccessMarks();

    void limitToPlans(const AbstractPlanGrp& active);
    const std::map<const AbstractPlan*, EntryPointGrp>& getSuccessMarks() const { return _successMarks; }

    void clear();
    const EntryPointGrp* succeededEntryPoints(const AbstractPlan* p) const;
    void removePlan(const AbstractPlan* plan);
    void markSuccessfull(const AbstractPlan* p, const EntryPoint* e);

    bool succeeded(const AbstractPlan* p, const EntryPoint* e) const;
    bool succeeded(int64_t planId, int64_t entryPointId) const;
    bool anyTaskSucceeded(const AbstractPlan* p) const;
    IdGrp toIdGrp() const;

  private:
    std::map<const AbstractPlan*, EntryPointGrp> _successMarks;
    const AlicaEngine* _ae;
};

} /* namespace alica */
