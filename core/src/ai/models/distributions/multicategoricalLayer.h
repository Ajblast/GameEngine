#pragma once

#include "common.h"
#include "ai/distributions/distribution.h"
#include <vector>
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace Decoders
			{
				// A categorical layer that creates distributions for different discrete action branches
				class multicategoricalLayerImpl : public torch::nn::Module
				{
				public:
					// Default Constructor
					multicategoricalLayerImpl() = default;
					// Create a multicategorical layer with the network input count and the branch sizes
					multicategoricalLayerImpl(int64 numInput, std::vector<int64> branchSizes);
					// Default Deconstructor
					virtual ~multicategoricalLayerImpl() = default;

					// Create a list of distributions. One for each branch
					std::vector<ref<Distributions::distribution>> forward(torch::Tensor inputs);
				private:
					torch::nn::ModuleList m_Branches;
				};
				TORCH_MODULE(multicategoricalLayer);
			}
		}
	}
}