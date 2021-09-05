#pragma once

#include "common.h"
#include "IAgentProgram.h"
#include "ai/actions/actuatorList.h"
#include "ai/sensors/ISensor.h"

namespace GRAVEngine
{
	namespace AI
	{
		class trainingProgram : public IAgentProgram
		{
		public:
			trainingProgram(std::string name, const std::vector<ref<Sensors::ISensor>>& sensors, Actions::actuatorList& actuators);
			virtual ~trainingProgram() = default;

			// Inherited via IAgentProgram
			virtual void requestDecision(agentInfo info, scope<ref<Sensors::ISensor> []>& sensors, size_t count) override;
			virtual Actions::actionBuffer decideAction() override;
			virtual void sendToDevice(inferenceDevice device) override;
		private:
			std::string m_Name;
			agentEpisodeId m_AgentId;
		};
	}
}