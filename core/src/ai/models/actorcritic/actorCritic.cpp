#include "gravpch.h"
#include "actorCritic.h"

GRAVEngine::AI::Models::ActorCritic::actorCriticImpl::actorCriticImpl(Training::networkSettings settings) : m_Critic(settings), m_Actor(settings)
{
	// Register the modules
	register_module("Critic", m_Critic);
	register_module("Actor", m_Actor);

	//pretty_print(std::cout);
}

std::tuple<GRAVEngine::AI::Models::ActorCritic::actorOutput, GRAVEngine::AI::Models::ActorCritic::criticOutput> GRAVEngine::AI::Models::ActorCritic::actorCriticImpl::forward(std::vector<torch::Tensor> inputs)
{
	// Run through both the actor actor and critic
	return { m_Actor->forward(inputs), m_Critic->forward(inputs) };
}

GRAVEngine::AI::Models::ActorCritic::agentLogProbs GRAVEngine::AI::Models::ActorCritic::actorCriticImpl::logProbs(std::vector<torch::Tensor> inputs, ActorCritic::agentAction action)
{
	return m_Actor->logProbs(inputs, action);
}
