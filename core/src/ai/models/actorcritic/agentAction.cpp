#include "gravpch.h"
#include "agentAction.h"

GRAVEngine::AI::Actions::actionBuffer GRAVEngine::AI::Models::ActorCritic::agentAction::toAgentBuffer() const
{
    // Create an array for the continuous actions
    size_t continuousCount = m_ContinuousActions.numel();
    scope<float[]> continuous = createScope<float[]>(continuousCount);

    // Copy the data if the continuous data exists
    if (continuousCount > 0)
    {
        auto continuousData = m_ContinuousActions.data_ptr<float>();

        // Copy the data into the array
        memcpy(continuous.get(), continuousData, continuousCount * sizeof(float));
    }


    // Create an array for the discrete actions
    size_t discreteCount = m_DiscreteActions.size();
    scope<int32[]> discrete = createScope<int32[]>(discreteCount);

    // Copy the decided action from each action tensor
    size_t offset = 0;
    for (auto it = m_DiscreteActions.begin(); it != m_DiscreteActions.end(); it++)
        discrete[offset] = it->item<int32>();

    // Create the segments
    ref<Actions::actionBufferSegment<float>> continuousSegment = createRef<Actions::actionBufferSegment<float>>(continuous, continuousCount);
    ref<Actions::actionBufferSegment<int32>> discreteSegment = createRef<Actions::actionBufferSegment<int32>>(discrete, discreteCount);

    // Make an action buffer
    return Actions::actionBuffer(continuousSegment, discreteSegment);
}
