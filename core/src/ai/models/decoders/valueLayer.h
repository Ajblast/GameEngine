#pragma once

#include "common.h"
#include "ai/models/actorcritic/criticOutput.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace Decoders
			{
				// A value layer will take the output from the network and get the values from it. e.g.., for the critic
				class valueLayerImpl : public torch::nn::Module
				{
				public:
					// Required default constructor
					valueLayerImpl() = default;
					// Create a value layer with the number of inputs and the number of outputs
					valueLayerImpl(int64 numInput, int64 numOutput = 1);
					// Default deconstructor
					virtual ~valueLayerImpl() = default;

					ActorCritic::criticOutput forward(torch::Tensor hidden);
				private:
					torch::nn::Linear m_Layer{ nullptr };	// The output layer
				};
				TORCH_MODULE(valueLayer);
			}
		}
	}
}