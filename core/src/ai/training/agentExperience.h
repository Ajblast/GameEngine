#pragma once

#include "common.h"
#include "ai/models/actorcritic/actorCriticOutput.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			// An experience of a single agent
			struct agentExperience
			{
			public:
				agentExperience(std::vector<torch::Tensor> observation, torch::Tensor reward, torch::Tensor done, Models::ActorCritic::actorCriticOputput actionInfo /*, torch::Tensor prevAction*/) :
					m_Observation(observation), m_Reward(reward), m_Done(done), m_ActionInformation(actionInfo)/*, m_PrevAction(prevAction)*/
				{}
			public:
				std::vector<torch::Tensor> m_Observation;						// The experience current observation
				torch::Tensor m_Reward;											// Reward for the observation
				torch::Tensor m_Done;											// Did the observation create a final state
				Models::ActorCritic::actorCriticOputput m_ActionInformation;	// Information about the action that was performed
				//torch::Tensor m_PrevAction;		// The previous action
			};
		}
	}
}