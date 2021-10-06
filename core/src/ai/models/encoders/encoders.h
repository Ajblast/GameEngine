#pragma once

#include "common.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace Encoders
			{
				// A default encoder
				class GRAVAPI encoderImpl : public torch::nn::Module
				{
				public:
					encoderImpl() = default;
					virtual ~encoderImpl() = default;

					// Default forward method that does nothing be returns the inputs
					virtual torch::Tensor forward(torch::Tensor inputs) { return inputs; }
				};
				TORCH_MODULE(encoder);

				// An encoder for a vector of inputs
				class GRAVAPI vectorEncoderImpl : public encoderImpl
				{
				public:
					vectorEncoderImpl() = default;
					virtual ~vectorEncoderImpl() = default;

					// Encode the 1D input. TODO: Potentially add a normalization ability. Right now just returns the inputs as is, which is the same as a default encoder
					virtual torch::Tensor forward(torch::Tensor inputs) override
					{
						return inputs;
					}
				};
				TORCH_MODULE(vectorEncoder);
			}
		}
	}
}