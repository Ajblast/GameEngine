#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace AI
	{
		// Parameters about an agent
		struct GRAVAPI agentParams
		{
		public:
			uint32 m_EpisodeCount;	// Number of episodes the agent has completed
			uint32 m_StepCount;		// Number of steps the agent has done this episode. Unique per agent
			uint32 m_MaxStepCount;	// Maximum number of steps an agent can take per episode
		};
	}
}