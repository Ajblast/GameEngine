#pragma once

#include "common.h"
#include "ai/models/actorcritic/agentAction.h"
#include "ai/models/actorcritic/agentLogProbs.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			// An update buffer takes a list of trajectories
			class GRAVAPI updateBuffer
			{
			public:
				updateBuffer() = default;
				~updateBuffer() = default;

				updateBuffer(updateBuffer&& other) noexcept;
				updateBuffer& operator=(updateBuffer&& other) noexcept;

				// Clear the update buffer
				void clear();
				// Append another update buffer to another. Copies data
				void append(const updateBuffer& other);

				// How many items are in the buffer. Based on observation counts, but all vectors should have the same amount of items in each
				inline const size_t size() const { return m_Observations.size() == 0 ? 0 : m_Observations.at(0).size(); }

				// Turn the observation map into a single observation
				std::vector<std::vector<torch::Tensor>> toObservation() const;
				// Turn the list of different actions into a single agent action
				Models::ActorCritic::agentAction toAgentAction() const;
				// Turn the list of logProbs into a single logProb
				Models::ActorCritic::agentLogProbs toLogProbs() const;

				// Create a minibatch subset from this buffer
				updateBuffer minibatch(size_t batchSize);

			public:
				// Store Observations based on their index from an initial observation list.
				// In this case, the vector does not store a single observation, but it stores a list of subparts from multiple subparts.
				// SOA no AOS
				std::map<size_t, std::vector<torch::Tensor>> m_Observations;

				std::vector<Models::ActorCritic::agentAction> m_Actions;	// The actions taken
				std::vector<Models::ActorCritic::agentLogProbs> m_LogProbs;	// The logProbs of the actions
				std::vector<torch::Tensor> m_Returns;						// The GAE return values
				std::vector<torch::Tensor> m_Advantages;					// The calculated advantages
			};
		}
	}
}