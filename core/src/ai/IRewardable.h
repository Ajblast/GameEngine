#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace AI
	{
		// Something that has rewards
		class IRewardable
		{
		public:
			IRewardable() = default;
			virtual ~IRewardable() = default;

			virtual void setReward(float reward) = 0;
			virtual void addReward(float increment) = 0;
			virtual float cumulativeReward() = 0;
		};
	}
}