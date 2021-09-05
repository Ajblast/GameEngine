#include "gravpch.h"
#include "actionLayer.h"

GRAVEngine::AI::Models::Decoders::actionLayerImpl::actionLayerImpl(int64 numInput, Actions::actionSpec actionSpec) : m_ContinuousLayer(nullptr), m_DiscreteLayer(nullptr), m_ActionSpec(actionSpec)
{
	// Create the continuous actions
	if (actionSpec.continuousActionCount() > 0)
	{
		m_ContinuousLayer = normalLayer(numInput, actionSpec.continuousActionCount());

		// Register the module
		register_module("Continuous Layer", m_ContinuousLayer);

	}

	// Create the discrete actions
	if (actionSpec.discreteActionCount() > 0)
	{
		// Copy the values from the action spec into a new vector to be passed
		std::vector<int64> branches;
		for (size_t i = 0; i < actionSpec.discreteActionCount(); i++)
			branches.push_back(actionSpec.branchSizes()[i]);

		// Create the layer
		m_DiscreteLayer = multicategoricalLayer(numInput, branches);

		// Register the module
		register_module("Discrete Layer", m_DiscreteLayer);
	}
}

std::tuple<GRAVEngine::AI::Models::ActorCritic::agentAction, GRAVEngine::AI::Models::ActorCritic::agentLogProbs, torch::Tensor> GRAVEngine::AI::Models::Decoders::actionLayerImpl::forward(torch::Tensor hidden)
{
	torch::Tensor sampledContinuous;
	torch::Tensor logProbContinuous;
	std::vector<torch::Tensor> sampledDiscrete;
	std::vector<torch::Tensor> logProbDiscrete;
	std::vector<torch::Tensor> entropies;

	// If there are continuous actions
	if (m_ActionSpec.continuousActionCount() > 0)
	{
		// Get the distribution
		auto continuousDist = m_ContinuousLayer->forward(hidden);

		// Sample it
		sampledContinuous = continuousDist->sample({ 1 });

		// Get the log prob of the sample
		logProbContinuous = continuousDist->logProb(sampledContinuous);

		// Get the entropy for the sample
		//std::cout << "Continuous Entropy: " << continuousDist->entropy() << std::endl;
		entropies.push_back(continuousDist->entropy());
	}

	// If there are discrete actions
	if (m_ActionSpec.discreteActionCount() > 0)
	{
		// Get the distributions
		auto discreteDist = m_DiscreteLayer->forward(hidden);

		// Sample the discrete action space
		for (auto it = discreteDist.begin(); it != discreteDist.end(); it++)
			sampledDiscrete.push_back((*it)->sample({ 1 }));

		// Get the log probs and entropies of the discrete actions
		for (size_t i = 0; i < discreteDist.size(); i++)
		{
			logProbDiscrete.push_back(discreteDist[i]->logProb(sampledDiscrete[i]));

			//std::cout << "Discrete Entropy " << i << ": " << discreteDist[i]->entropy() << std::endl;
			entropies.push_back(discreteDist[i]->entropy());
		}
	}
	
	// Create the agentAction
	ActorCritic::agentAction actions = { sampledContinuous, sampledDiscrete };

	// Create the logProb
	ActorCritic::agentLogProbs logProbs = { logProbContinuous , logProbDiscrete };

	// Concatenate all of the entropies into t a single tensor
	torch::Tensor entropy = torch::cat(entropies, 1);

	// Return tuple of the items
	return { actions, logProbs, entropy };
}

GRAVEngine::AI::Models::ActorCritic::agentLogProbs GRAVEngine::AI::Models::Decoders::actionLayerImpl::logProbs(torch::Tensor hidden, ActorCritic::agentAction action)
{
	torch::Tensor logProbContinuous;
	std::vector<torch::Tensor> logProbDiscrete;

	// If there are continuous actions
	if (m_ActionSpec.continuousActionCount() > 0)
	{
		// Get the distribution
		auto continuousDist = m_ContinuousLayer->forward(hidden);

		// Get the log prob of the sample
		logProbContinuous = continuousDist->logProb(action.m_ContinuousActions);
	}

	// If there are discrete actions
	if (m_ActionSpec.discreteActionCount() > 0)
	{
		// Get the distributions
		auto discreteDist = m_DiscreteLayer->forward(hidden);

		// Get the log probs and entropies of the discrete actions
		for (size_t i = 0; i < discreteDist.size(); i++)
			logProbDiscrete.push_back(discreteDist[i]->logProb(action.m_DiscreteActions[i]));
	}

	return { logProbContinuous, logProbDiscrete };
}
