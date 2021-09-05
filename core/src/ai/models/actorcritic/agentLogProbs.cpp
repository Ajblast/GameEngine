#include "gravpch.h"
#include "agentLogProbs.h"

torch::Tensor GRAVEngine::AI::Models::ActorCritic::agentLogProbs::flatten()
{
    std::vector<torch::Tensor> logProbs;
    logProbs.insert(logProbs.end(), m_ContinuousActions);
    logProbs.insert(logProbs.end(), m_DiscreteActions.begin(), m_DiscreteActions.end());

    // Combine the logprobs into a single dimensional tensor
    return torch::cat(logProbs, 1);
}
