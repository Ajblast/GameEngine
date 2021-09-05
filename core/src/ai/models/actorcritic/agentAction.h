#pragma once

#include "common.h"
#include "ai/actions/actionBuffer.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace ActorCritic
			{
				// A small struct that holds all of the discrete and continuous actions
				struct agentAction
				{
				public:
					Actions::actionBuffer toAgentBuffer() const;
				public:
					torch::Tensor m_ContinuousActions;				// The continuous actions
					std::vector<torch::Tensor> m_DiscreteActions;	// List of tensors. Each tensor correlates to a different branch
				};
			}
		}
	}
}