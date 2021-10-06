#pragma once

#include "common.h"
#include "ai/agentInfo.h"
#include "ai/sensors/ISensor.h"
#include "ai/actions/actionBuffer.h"
#include "ai/inferenceDevice.h"

namespace GRAVEngine
{
	namespace AI
	{ 
		// Program for controlling an agent's actions
		class GRAVAPI IAgentProgram
		{
		public:
			IAgentProgram() = default;
			virtual ~IAgentProgram() = default;

			// Have the agent request a decision. The decision is not done until decideAction is called.
			virtual void requestDecision(agentInfo info, scope<ref<Sensors::ISensor>[]>& sensors, size_t count) = 0;
			// If the decision has not been completed yet, complete the action
			virtual Actions::actionBuffer decideAction() = 0;

			// Send the program to a certain device
			virtual void sendToDevice(inferenceDevice device) = 0;
		};
	}
}