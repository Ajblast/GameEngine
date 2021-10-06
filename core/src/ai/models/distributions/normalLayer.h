#pragma once

#include "common.h"
#include "ai/distributions/distribution.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace Decoders
			{
				// A normal layer that creates a distribution for a continuous action space
				class GRAVAPI normalLayerImpl : public torch::nn::Module
				{
				public:
					// Default Constructor
					normalLayerImpl() = default;
					// Create a multicategorical layer with the network input count and the branch sizes
					normalLayerImpl(int64 numInput, int64 numContinuous);
					// Default Deconstructor
					virtual ~normalLayerImpl() = default;

					// Create a normal distribution from the input
					ref<Distributions::distribution> forward(torch::Tensor inputs);
				private:
					torch::nn::Linear m_Mu{ nullptr };
					torch::Tensor m_Log;
				};
				TORCH_MODULE(normalLayer);
			}
		}
	}
}