#include "gravpch.h"
#include "agentInfo.h"

GRAVEngine::AI::agentInfo::agentInfo() :
	m_IsDone(false), m_Reward(0), m_CumulativeReward(0), m_EpisodeID(0), m_maxStepReached(false), m_Actions()
{
}

void GRAVEngine::AI::agentInfo::clearActions()
{
	m_Actions.clear();
}

void GRAVEngine::AI::agentInfo::copyActions(Actions::actionBuffer buffer)
{
	m_Actions.copy(buffer);
}
