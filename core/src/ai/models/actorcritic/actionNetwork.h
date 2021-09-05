#pragma once

#include "common.h"
#include "actor.h"
#include "ai/training/networkSettings.h"
#include "ai/actions/actionSpec.h"
#include "ai/models/network.h"
#include "ai/models/decoders/actionLayer.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace ActorCritic
			{
				// Module that represents a simple actor
				class actionNetworkImpl : public actorImpl
				{
				public:
					// Required default constructor
					actionNetworkImpl() = default;
					// Create a default critic that will take sensor input and 
					actionNetworkImpl(Training::networkSettings settings);

					// Default deconstructor
					virtual ~actionNetworkImpl() = default;

					// Using the inputs, determine the actions, the log probabilities, and the entropy
					virtual actorOutput forward(std::vector<torch::Tensor> inputs) override;

					// Get the log probs based on a state
					virtual agentLogProbs logProbs(std::vector<torch::Tensor> inputs, agentAction action) override;
				private:
					network m_Network;						// The network body
					Decoders::actionLayer m_ActionLayer;	// The final action layer
				};
				TORCH_MODULE(actionNetwork);
			}
		}
	}
}