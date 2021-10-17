#pragma once

#include "common.h"
#include "inferenceDevice.h"
#include "agentPrograms/IAgentProgram.h"
#include "actions/actionSpec.h"
#include "actions/actuatorList.h"
#include "ai/models/model.h"

namespace GRAVEngine
{
	namespace AI
	{
		// Parameters for agent behavior and brain properties
		// Used mostly for debugging and holding the actual nn model that is passed around
		struct GRAVAPI programParams
		{
		public:
			programParams(inferenceDevice device, const std::string& name, const std::string& folderPath, ref<Models::model> model = nullptr);

			// If the model is null, a training program is created. If the model is set, then an inference program is created.
			scope<IAgentProgram> createProgram(const std::vector<ref<Sensors::ISensor>>& sensors, Actions::actuatorList& actuators);

		public:
			ref<Models::model> m_Model;			// Agent model
			inferenceDevice m_InferenceDevice;	// Inference device
			std::string m_Name;					// Name of the agent
			std::string m_FolderPath;			// Path of the folder
		};
	}
}