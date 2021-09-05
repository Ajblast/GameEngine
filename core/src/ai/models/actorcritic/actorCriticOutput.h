#pragma once

#include "common.h"
#include "actorOutput.h"
#include "criticOutput.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace ActorCritic
			{
				// The output of an actor critic.
				typedef std::tuple<actorOutput, criticOutput> actorCriticOputput;
			}
		}
	}
}