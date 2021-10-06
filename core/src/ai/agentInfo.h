#pragma once

#include "common.h"
#include "actions/actionBuffer.h"
#include "agentEpisodeId.h"

namespace GRAVEngine
{
	namespace AI
	{
		// Information about an agent
		struct GRAVAPI agentInfo
		{
		public:
			agentInfo();
			// Clear the action buffer
			void clearActions();
			// Copy another buffer into this buffer. Discrete and continuous counts must be equal
			void copyActions(Actions::actionBuffer buffer);

		public:
			bool m_IsDone;						// Is the agent done after the last actions it took
			Actions::actionBuffer m_Actions;	// The actions the agent previously took
			float m_Reward;						// The agent's current reward
			float m_CumulativeReward;			// The current cumulative reward
			agentEpisodeId m_EpisodeID;			// Agent's ID in the current episode
			bool m_maxStepReached;				// Has the maximum number of steps been reached
		};
	}
}