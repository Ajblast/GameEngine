#pragma once

#include "common.h"
#include "agentAction.h"
#include "agentLogProbs.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace ActorCritic
			{
				// An actor outputs the decided actions, their log probabilities, and the entropy
				typedef std::tuple<agentAction, agentLogProbs, torch::Tensor> actorOutput;
			}
		}
	}
}