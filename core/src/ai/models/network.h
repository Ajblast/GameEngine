#pragma once

#include "common.h"
#include "ai/training/networkSettings.h"
#include "inputLayer.h"
#include "linearLayers.h"
#include <vector>
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			// A general neural network that will take a list of observations and translate them into vlaues
			class GRAVAPI networkImpl : public torch::nn::Module
			{
			public:
				// Required default constructor
				networkImpl() = default;
				// A network must be created with network settings
				networkImpl(Training::networkSettings settings);
				// Default deconstructor
				virtual ~networkImpl() = default;

				// Take a list of inputs and run them through the network
				torch::Tensor forward(std::vector<torch::Tensor> inputs);
			private:
				inputLayer m_InputLayer;		// The input layer
				linearLayers m_LinearLayers;	// The hidden layers
			};
			TORCH_MODULE(network);
		}
	}
}