#pragma once

#include "common.h"
#include "criticOutput.h"
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
				// An abstract critic needed for an actor-acritic setup
				class GRAVAPI criticImpl : public torch::nn::Module
				{
				public:
					// Required default constructor
					criticImpl() = default;
					// Default deconstructor
					virtual ~criticImpl() = default;

					virtual criticOutput forward(std::vector<torch::Tensor> inputs)
					{
						return criticOutput();
					}
				};
				TORCH_MODULE(critic);
			}
		}
	}
}