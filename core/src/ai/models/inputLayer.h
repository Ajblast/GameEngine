#pragma once

#include "common.h"
#include "ai/sensors/observationParams.h"
#include "ai/training/networkSettings.h"
#include <vector>
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			// A module that represents the input to a network body
			class GRAVAPI inputLayerImpl : public torch::nn::Module
			{
			public:
				inputLayerImpl() = default;
				// Creates all of the input encoders given the observation specs. Only currently supports 1d vectors currently
				inputLayerImpl(Training::networkSettings settings);
				// Default deconstructor
				virtual ~inputLayerImpl() = default;

				// Given a list of input tensors (Sensor information), get a single tensor of all of the input information.
				// Encodes all of the input information
				torch::Tensor forward(std::vector<torch::Tensor> inputs);

				inline size_t totalInputSize() const { return m_TotalInputSize.item<int64>(); }
			private:
				torch::nn::ModuleList m_Modules;	// A list containing all of the input encoder modules

				torch::Tensor m_TotalInputSize;		// The total input size of all of the inputs
			};

			TORCH_MODULE(inputLayer);
		}
	}
}