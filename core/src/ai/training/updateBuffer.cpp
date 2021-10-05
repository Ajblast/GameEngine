#include "gravpch.h"
#include "updateBuffer.h"
#include "utils/randomInt.h"

GRAVEngine::AI::Training::updateBuffer::updateBuffer(updateBuffer&& other) noexcept :
	m_Observations(std::move(other.m_Observations)), m_Actions(std::move(other.m_Actions)), m_LogProbs(std::move(other.m_LogProbs)), m_Advantages(std::move(other.m_Advantages)), m_Returns(std::move(other.m_Returns))
{
}

GRAVEngine::AI::Training::updateBuffer& GRAVEngine::AI::Training::updateBuffer::operator=(updateBuffer&& other) noexcept
{
	if (this != &other)
	{
		m_Observations = std::move(other.m_Observations);
		m_Actions = std::move(other.m_Actions);
		m_LogProbs = std::move(other.m_LogProbs);
		m_Advantages = std::move(other.m_Advantages);
		m_Returns = std::move(other.m_Returns);
	}
	return *this;
}

void GRAVEngine::AI::Training::updateBuffer::clear()
{
	m_Observations.clear();
	m_Actions.clear();
	m_LogProbs.clear();
	m_Advantages.clear();
	m_Returns.clear();
}

void GRAVEngine::AI::Training::updateBuffer::append(const updateBuffer& other)
{
	// Copy all of the data from the other update buffer
	for (auto it = other.m_Observations.begin(); it != other.m_Observations.end(); it++)
		m_Observations[it->first].insert(m_Observations[it->first].end(), it->second.begin(), it->second.end());

	m_Actions.insert(m_Actions.end(), other.m_Actions.begin(), other.m_Actions.end());
	m_LogProbs.insert(m_LogProbs.end(), other.m_LogProbs.begin(), other.m_LogProbs.end());
	m_Advantages.insert(m_Advantages.end(), other.m_Advantages.begin(), other.m_Advantages.end());
	m_Returns.insert(m_Returns.end(), other.m_Returns.begin(), other.m_Returns.end());
}

std::vector<std::vector<torch::Tensor>> GRAVEngine::AI::Training::updateBuffer::toObservation() const
{
	std::vector<std::vector<torch::Tensor>> observation;
	// Add each observation to the overall observation
	for (auto it = m_Observations.begin(); it != m_Observations.end(); it++)
	{
		observation.push_back(it->second);
	}

	// Return the overall observation
	return observation;
}

GRAVEngine::AI::Models::ActorCritic::agentAction GRAVEngine::AI::Training::updateBuffer::toAgentAction() const
{
	// Get all of the continuous actions
	std::vector<torch::Tensor> continuousActions;
	for (auto it = m_Actions.begin(); it != m_Actions.end(); it++)
		if (it->m_ContinuousActions.defined())
			continuousActions.push_back(it->m_ContinuousActions);

	// Combine the continous actions into a single tensor
	torch::Tensor continuous = torch::Tensor();
	if (continuousActions.size() > 0)
		continuous = torch::cat(continuousActions);

	// For each action, store each branch based on its index into the orignal vector (branch)
	// This is so that each item in the map will be a part of each discrete action so we can concatonate each one
	std::map<size_t, std::vector<torch::Tensor>> discreteActions;
	for (auto it = m_Actions.begin(); it != m_Actions.end(); it++)
		for (size_t i = 0; i < it->m_DiscreteActions.size(); i++)
			discreteActions[i].push_back(it->m_DiscreteActions[i]);

	// Concate each branch
	std::vector<torch::Tensor> discrete;
	for (auto it = discreteActions.begin(); it != discreteActions.end(); it++)
		discrete.push_back(torch::cat(it->second));

	// Return the combined agentAction
	return { continuous, discrete };
}

GRAVEngine::AI::Models::ActorCritic::agentLogProbs GRAVEngine::AI::Training::updateBuffer::toLogProbs() const
{
	// Get all of the continuous log probs
	std::vector<torch::Tensor> continousLogProbs;
	for (auto it = m_LogProbs.begin(); it != m_LogProbs.end(); it++)
		if (it->m_ContinuousActions.defined())
			continousLogProbs.push_back(it->m_ContinuousActions);

	// Combine the continous logProbs into a single tensor
	torch::Tensor continuous = torch::Tensor();
	if (continousLogProbs.size() > 0)
		continuous = torch::cat(continousLogProbs);

	// For each logProb, store each branch based on its index into the orignal vector (branch)
	// This is so that each item in the map will be a part of each discrete logProb so we can concatonate each one
	std::map<size_t, std::vector<torch::Tensor>> discreteLogProbs;
	for (auto it = m_LogProbs.begin(); it != m_LogProbs.end(); it++)
		for (size_t i = 0; i < it->m_DiscreteActions.size(); i++)
			discreteLogProbs[i].push_back(it->m_DiscreteActions[i]);

	// Concate each branch
	std::vector<torch::Tensor> discrete;
	for (auto it = discreteLogProbs.begin(); it != discreteLogProbs.end(); it++)
		discrete.push_back(torch::cat(it->second));

	// Return the combined agentLogProbs
	return { continuous, discrete };
}

GRAVEngine::AI::Training::updateBuffer GRAVEngine::AI::Training::updateBuffer::minibatch(size_t batchSize)
{
	updateBuffer buffer;

	// Get the indices that will be copied
	std::vector<size_t> indices = randomInt((size_t) 0, size() - 1, batchSize);

	for (auto it = indices.begin(); it != indices.end(); it++)
	{
		for (auto ob = m_Observations.begin(); ob != m_Observations.end(); ob++)
		{
			buffer.m_Observations[ob->first].push_back(ob->second[*it]);
		}

		// Add the simple indexable items
		buffer.m_Actions.push_back(m_Actions[*it]);
		buffer.m_LogProbs.push_back(m_LogProbs[*it]);
		buffer.m_Advantages.push_back(m_Advantages[*it]);
		buffer.m_Returns.push_back(m_Returns[*it]);
	}

	return buffer;
}
