#pragma once

#include "common.h"
#include "valueNetwork.h"
#include "actionNetwork.h"
#include "ai/models/model.h"
#include "actorCriticOutput.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace ActorCritic
			{
				// A module that represents both the actor and critic in an actor critic model
				class GRAVAPI actorCriticImpl : public modelImpl
				{
				public:
					// Required default constructor
					actorCriticImpl() = default;
					// Create an actor critic model
					actorCriticImpl(Training::networkSettings settings);
					// Default deconstructor
					virtual ~actorCriticImpl() = default;

					// Pass through the actor and critic and get their respective outputs
					virtual actorCriticOputput forward(std::vector<torch::Tensor> inputs) override;

					// Get the log probs based on a state
					virtual agentLogProbs logProbs(std::vector<torch::Tensor> inputs, agentAction action) override;
				private:
					valueNetwork m_Critic;	// The critic
					actionNetwork m_Actor;	// The actor
				};
				TORCH_MODULE(actorCritic);
			}
		}
	}
}