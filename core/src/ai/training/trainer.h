#pragma once

#include "common.h"
#include "ITrainingAlgorithm.h"
#include "ai/agentEpisodeId.h"
#include "ai/agentInfo.h"
#include "ai/actions/actionBuffer.h"
#include "ai/models/actorcritic/actorCriticOutput.h"
#include "ai/inferenceDevice.h"
#include "trainerSettings.h"
#include "locks/scopedLock.h"
#include "locks/spinLock.h"
#include "jobs/jobs.h"
#include <map>
#include <vector>
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			class GRAVAPI trainer
			{
			public:
				trainer(trainerSettings settings, scope<ITrainingAlgorithm> algorithm);
				// Move constructor is deleted because of potential deadlock because multiple threads can talk to the same trainer
				trainer(trainer&& other) noexcept = delete;
				// Delete the copy constructor
				trainer(const trainer& other) = delete;

				trainer& operator=(trainer&& other) noexcept;
				trainer& operator=(const trainer& other) = delete;

				// Add an observation to the trainer for a specific agent along with the information about the previous state found in agentInfo
				// This will also save the actions for later retrieval
				void addObservation(agentEpisodeId agentId, agentInfo info, std::vector<torch::Tensor> observation);
				// Run a single step through the trainer. This may cause learning to occur and also parse all agents needing a decision
				void step();
				// Reset the trainer
				void reset();
				// Get the decided actions for a specific agent
				const Actions::actionBuffer getActions(agentEpisodeId agentId);

				// Print the model
				void printModel() const;

				// Send the model to a device
				void sendToDevice(inferenceDevice device) const;
			private:
				trainerSettings m_Settings;												// Trainer settings
				scope<ITrainingAlgorithm> m_Algorithm;									// Learning algorithm of the trainer
				Locks::spinLock m_SpinLock;

				//std::vector<agentEpisodeId> m_AgentsRequestingDecision;					// Agents that are currently requesting a decision to be made for them
				std::map<agentEpisodeId, Actions::actionBuffer> m_DecidedAgentActions;	// Actions decided for each agent

				std::map<agentEpisodeId, std::vector<agentExperience>> m_Experiences;	// Agent experiences


				std::map<agentEpisodeId, std::tuple<std::vector<torch::Tensor>, agentInfo, Models::ActorCritic::actorCriticOputput>> m_LastObservation;
				std::map<agentEpisodeId, float> m_CumulativeRewards;	// Cumulative reward of the agent
				std::map<agentEpisodeId, size_t> m_Steps;				// Total number of steps an agent takes

				std::vector<trajectory> m_Trajectories;	// Trajectories created from agents adding observations
			};
		}
	}
}