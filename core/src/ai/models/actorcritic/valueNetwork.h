#pragma once

#include "common.h"
#include "ai/training/networkSettings.h"
#include "critic.h"
#include "ai/models/network.h"
#include "ai/models/decoders/valueLayer.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace ActorCritic
			{
				class GRAVAPI valueNetworkImpl : public criticImpl
				{
				public:
					// Required default constructor
					valueNetworkImpl() = default;
					// Create a default critic that will take sensor input and 
					valueNetworkImpl(Training::networkSettings settings);

					// Default deconstructor
					virtual ~valueNetworkImpl() = default;

					virtual criticOutput forward(std::vector<torch::Tensor> inputs) override;
				private:
					network m_Network;					// The network body
					Decoders::valueLayer m_ValueLayer;	// The final value layer
				};
				TORCH_MODULE(valueNetwork);
			}
		}
	}
}