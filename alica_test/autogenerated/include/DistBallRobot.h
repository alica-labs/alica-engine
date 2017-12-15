/*
 * DistBallRobot.h
 *
 *  Created on: Oct 27, 2014
 *      Author: Stefan Jakob
 */

#ifndef ALICA_ALICA_TEST_AUTOGENERATED_SRC_DISTBALLROBOT_H_
#define ALICA_ALICA_TEST_AUTOGENERATED_SRC_DISTBALLROBOT_H_


#include <engine/USummand.h>

#include <vector>
#include <string>
#include <memory>

using namespace std;

namespace supplementary
{
    class AgentID;
    class AgentIDManager;
}

namespace alica
{

	class UtilityInterval;
	class IAssignment;

	class DistBallRobot : public USummand
	{
	public:
		DistBallRobot(double weight, string name, long id, vector<long>& relevantEntryPointIds);
		virtual ~DistBallRobot();
		void cacheEvalData ();
		UtilityInterval eval(IAssignment* ass);
		supplementary::AgentID * robotId;

	protected:
		double sb;
		double angleBallOpp;
		double velAngle;
		supplementary::AgentIDManager* manager;
	};

} /* namespace alica */

#endif /* ALICA_ALICA_TEST_AUTOGENERATED_SRC_DISTBALLROBOT_H_ */
