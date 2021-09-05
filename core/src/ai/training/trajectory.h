#pragma once

#include "common.h"
#include "agentExperience.h"
#include "updateBuffer.h"
#include <vector>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			// A trajectory is an ordered list of agent experiences that describe a set of realated N+H actions
			class trajectory
			{
			public:
				trajectory(const std::vector<agentExperience>& experiences/*, torch::Tensor nextObservation*/) :
					m_Experiences(experiences.begin(), experiences.end())/*, m_NextObservation(nextObservation)*/
				{}

				updateBuffer toUpdateBuffer() const ;

				inline const size_t size() const { return m_Experiences.size(); }
			public:
				std::vector<agentExperience> m_Experiences;	// Experiences
				//torch::Tensor m_NextObservation;			// The next observation for the trajectory.
			};
		}
	}
}