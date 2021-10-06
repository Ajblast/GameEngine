#pragma once

#include "common.h"
#include "IAgentProgram.h"
#include "ai/actions/actuatorList.h"
#include "ai/sensors/ISensor.h"
#include "ai/models/model.h"

namespace GRAVEngine
{
	namespace AI
	{
		class GRAVAPI inferenceProgram : public IAgentProgram
		{
		public:
			inferenceProgram(ref<Models::model> model);
			virtual ~inferenceProgram() = default;

			// Inherited via IAgentProgram
			virtual void requestDecision(agentInfo info, scope<ref<Sensors::ISensor>[]>& sensors, size_t count) override;
			virtual Actions::actionBuffer decideAction() override;
			virtual void sendToDevice(inferenceDevice device) override;
		private:
			ref<Models::model> m_Model;
			Actions::actionBuffer m_Buffer;
		};
	}
}