#pragma once

#include "common.h"
#include "actorcritic/actorCriticOutput.h"
#include <string>
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			// An abstract model for AI
			class modelImpl : public torch::nn::Module
			{
			public:
				// Required default constructor
				modelImpl() = default;
				// Default deconstructor
				virtual ~modelImpl() = default;

			public:
				// Load a model from a file and take its weights, parameters, and buffers
				virtual void load(std::string filePath)
				{
				}
				// Save a model with its weights, parameters, and buffers
				virtual void save(std::string filePath)
				{
				}

				// Run through the model
				virtual ActorCritic::actorCriticOputput forward(std::vector<torch::Tensor> inputs)
				{
					return ActorCritic::actorCriticOputput();
				}

				// Get the log probs based on a state
				virtual ActorCritic::agentLogProbs logProbs(std::vector<torch::Tensor> inputs, ActorCritic::agentAction action) {
					return ActorCritic::agentLogProbs();
				}
			};
			TORCH_MODULE(model);
		}
	}
}