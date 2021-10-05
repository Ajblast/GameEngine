#include "gravpch.h"
#include "trajectory.h"

GRAVEngine::AI::Training::updateBuffer GRAVEngine::AI::Training::trajectory::toUpdateBuffer() const
{
    updateBuffer buffer;

	// Iterate over each observation and add the parts except for the last one.
	// The last one is reserved as the terminal state
	for (auto it = m_Experiences.begin(); it != m_Experiences.end() - 1; it++)
	{
		// Add each individual subpart of the observation. Struct of arrays, not array of structs
		for (size_t i = 0; i < it->m_Observation.size(); i++)
			buffer.m_Observations[i].push_back(it->m_Observation[i]);

		buffer.m_Actions.push_back(std::get<0>(std::get<0>(it->m_ActionInformation)));
		buffer.m_LogProbs.push_back(std::get<1>(std::get<0>(it->m_ActionInformation)));
		//buffer.m_Values.push_back(std::get<1>(it->m_ActionInformation));
		//buffer.m_Rewards.push_back(it->m_Reward);
		//buffer.m_Dones.push_back(it->m_Done);
	}
    return buffer;
}
