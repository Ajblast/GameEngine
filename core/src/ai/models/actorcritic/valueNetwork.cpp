#include "gravpch.h"
#include "valueNetwork.h"

GRAVEngine::AI::Models::ActorCritic::valueNetworkImpl::valueNetworkImpl(Training::networkSettings settings) : m_Network(settings), m_ValueLayer(settings.m_HiddenCount)
{
	// Register the modules
	register_module("Network", m_Network);
	register_module("ValueLayer", m_ValueLayer);
}

GRAVEngine::AI::Models::ActorCritic::criticOutput GRAVEngine::AI::Models::ActorCritic::valueNetworkImpl::forward(std::vector<torch::Tensor> inputs)
{
	// Send the inputs through the network body
	torch::Tensor inputIncodings = m_Network->forward(inputs);

	// Get the values from the input incodings
	return m_ValueLayer->forward(inputIncodings);
}
