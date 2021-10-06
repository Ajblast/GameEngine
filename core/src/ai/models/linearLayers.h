#pragma once

#include "common.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			// A module that represents the body to a network body. All of the linear layers
			class GRAVAPI linearLayersImpl : public torch::nn::Module
			{
			public:
				// Default linear layers. Empty sequential module
				linearLayersImpl() = default;
				// Create all of the linear layers
				linearLayersImpl(int64 numInputs, int64 numLayers, int64 hiddenSize);
				// Default deconstructor
				virtual ~linearLayersImpl() = default;

				// Pass the tensor input through all of the linear layers
				torch::Tensor forward(torch::Tensor inputs);

			private:
				torch::nn::Sequential m_Layers; // A sequential module containing all of the layers
			};
			TORCH_MODULE(linearLayers);
		}
	}
}