/*
 * ForallAgents.h
 *
 *  Created on: Mar 5, 2014
 *      Author: Stephan Opfer
 */

#ifndef FORALLAGENTS_H_
#define FORALLAGENTS_H_

#include <list>
#include <vector>
#include <memory>


#include "Quantifier.h"

namespace alica
{

	class RunningPlan;
	class Variable;
	class AlicaEngine;
	class SolverTerm;

	/**
	 * A quantifier associated with agents, i.e., the domain identifiers of this quantifier refer to properties of an agent
	 */
	class ForallAgents : public Quantifier
	{
	public:
		ForallAgents(AlicaEngine* ae, long id = 0);
		virtual ~ForallAgents();
		shared_ptr<list<vector<Variable* > > > getDomainVariables(shared_ptr<RunningPlan>& p, shared_ptr<vector<int> >& agentsInScope);

	protected:
		AlicaEngine* ae;


	};

} /* namespace Alica */

#endif /* FORALLAGENTS_H_ */
