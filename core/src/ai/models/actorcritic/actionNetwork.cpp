#include "gravpch.h"
#include "actionNetwork.h"

GRAVEngine::AI::Models::ActorCritic::actionNetworkImpl::actionNetworkImpl(Training::networkSettings settings) : m_Network(settings), m_ActionLayer(settings.m_HiddenCount, settings.m_ActionSpec)
{
    // Register the modules
    register_module("Action Network", m_Network);
    register_module("Action Layer", m_ActionLayer);
}

GRAVEngine::AI::Models::ActorCritic::actorOutput GRAVEngine::AI::Models::ActorCritic::actionNetworkImpl::forward(std::vector<torch::Tensor> inputs)
{
    // Run through the netowrk
    torch::Tensor encoding = m_Network->forward(inputs);

    // Get the actions
    return m_ActionLayer->forward(encoding);
}

GRAVEngine::AI::Models::ActorCritic::agentLogProbs GRAVEngine::AI::Models::ActorCritic::actionNetworkImpl::logProbs(std::vector<torch::Tensor> inputs, agentAction action)
{
    // Run through the network
    torch::Tensor encoding = m_Network->forward(inputs);

    // Get the logProbs
    return m_ActionLayer->logProbs(encoding, action);
}
