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
			class GRAVAPI modelImpl : public torch::nn::Module
			{
			public:
				// Required default constructor
				modelImpl() = default;
				// Default deconstructor
				virtual ~modelImpl() = default;

			public:
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

			template<typename Model, typename Optimizer>
			void load(Model model, Optimizer& optimizer, const std::string& filePath)
			{
				torch::load(model, filePath + "Model.pt");
				torch::load(optimizer, filePath + "Optimizer.pt");
			}
			template<typename Model, typename Optimizer>
			void save(Model model, Optimizer& optimizer, const std::string& filePath)
			{
				torch::save(model, filePath + "Model.pt");
				torch::save(optimizer, filePath + "Optimizer.pt");
			}

		}
	}
}