#pragma once

#include "common.h"
#include "actorOutput.h"
#include <vector>
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace ActorCritic
			{
				// The actor of an actor-critic network
				class GRAVAPI actorImpl : public torch::nn::Module
				{
				public:
					// Required default constructor
					actorImpl() = default;
					// Default deconstructor
					virtual ~actorImpl() = default;

					// Get the actions, log probs, and entropy
					virtual actorOutput forward(std::vector<torch::Tensor> inputs)
					{
						return actorOutput();
					}

					// Get the log probs based on a state
					virtual agentLogProbs logProbs(std::vector<torch::Tensor> inputs, agentAction action)
					{
						return agentLogProbs();
					}
				};
				TORCH_MODULE(actor);
			}
		}
	}
}