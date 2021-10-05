#include "gravpch.h"
#include "PPO.h"

GRAVEngine::AI::Training::Algorithms::PPO::PPO(networkSettings settings, ref<ppoHyperparameters> hyperparameters) :
    m_Model(createRef<Models::ActorCritic::actorCritic>(settings)),
    m_Hyperparameters(hyperparameters),
    m_Optimizer(m_Model->operator->()->parameters(), torch::optim::AdamOptions(0.0001).betas({ 0.5, 0.999 }))
{
    GRAV_ASSERT(hyperparameters != nullptr);    // Make sure that the hyperparameters are actually passed in
}

//GRAVEngine::AI::Models::model& GRAVEngine::AI::Training::Algorithms::PPO::model()
//{
//    // Cast the actor-critic into a generic model
//    return &(*m_Model);
//}
GRAVEngine::AI::Training::algorithmType GRAVEngine::AI::Training::Algorithms::PPO::getAlgorithmType()
{
    return algorithmType::PPO;
}
GRAVEngine::AI::Training::networkSettings GRAVEngine::AI::Training::Algorithms::PPO::getNetworkSettings()
{
    // Create a copy of the network settings
    return m_Settings;
}
GRAVEngine::AI::Training::hyperparameters& GRAVEngine::AI::Training::Algorithms::PPO::getHyperparameters()
{
    return *m_Hyperparameters;
}
bool GRAVEngine::AI::Training::Algorithms::PPO::shouldUpdate()
{
    if (m_UpdateBuffer.size() < m_Hyperparameters->m_BufferSize)
        return false;

    return true;
}
//bool GRAVEngine::AI::Training::Algorithms::PPO::initialized()
//{
//    return false;
//}

GRAVEngine::AI::Models::ActorCritic::actorCriticOputput GRAVEngine::AI::Training::Algorithms::PPO::forward(std::vector<torch::Tensor> inputs)
{
    return m_Model->operator->()->forward(inputs);
}

void GRAVEngine::AI::Training::Algorithms::PPO::print() const
{
    std::stringstream ss;

    ss << *m_Model->operator->() << std::endl;

    GRAV_LOG_LINE_INFO("\nPrint Model:\n%s", ss.str().c_str());

}
void GRAVEngine::AI::Training::Algorithms::PPO::update()
{
    // Cheeck if there are enough update experiences to be able to update
    if (shouldUpdate() == false)
        return;

    size_t batchSize = m_UpdateBuffer.size();               // Total batch size
    size_t miniBatchSize = m_Hyperparameters->m_BatchSize;  // Size of the minibatches
    float clip = m_Hyperparameters->m_Clip;                 // Clipping parameter

    // Run for the specified epoch count
    for (size_t i = 0; i < m_Hyperparameters->m_EpochCount; i++)
    {
        // For as many minibatches that can be created
        for (size_t j = 0; j < batchSize / miniBatchSize; j++)
        {
            // Get a random sample from the update buffer batchSize long
            updateBuffer miniBatch = m_UpdateBuffer.minibatch(miniBatchSize);


            // Use that sample to update

            // Turn the map of observations into a single observation
            std::vector<std::vector<torch::Tensor>> observationBuffer = miniBatch.toObservation();
            std::vector<torch::Tensor> observation;
            for (auto it = observationBuffer.begin(); it != observationBuffer.end(); it++)
                observation.push_back(torch::cat(*it));

            // Combine all of the actions into a single agentAction
            Models::ActorCritic::agentAction actions = miniBatch.toAgentAction();
            Models::ActorCritic::agentLogProbs oldlogProbs = miniBatch.toLogProbs();

            // Combine to single advantage
            torch::Tensor advantage = torch::cat(miniBatch.m_Advantages);
            torch::Tensor returns = torch::cat(miniBatch.m_Returns);

            //GRAV_LOG_LINE_INFO("Advantage");
            //advantage.print();
            //GRAV_LOG_LINE_INFO("Returns");
            //returns.print();

            // Run through the model
            Models::ActorCritic::actorCriticOputput output = m_Model->operator->()->forward(observation);
            torch::Tensor value = std::get<1>(output);
            torch::Tensor entropy = std::get<2>(std::get<0>(output));

            //GRAV_LOG_LINE_INFO("Critic Value");
            //value.print();
            //GRAV_LOG_LINE_INFO("Distribution Entropy");
            //torch::print(entropy);
            //GRAV_LOG_LINE_INFO("");

            Models::ActorCritic::agentLogProbs newLogProbs = m_Model->operator->()->logProbs(observation, actions);


            // Calculate the surrogates
            torch::Tensor ratio = (newLogProbs.flatten() - oldlogProbs.flatten()).exp();
            torch::Tensor surr1 = ratio * advantage;
            torch::Tensor surr2 = torch::clamp(ratio, 1.0 - clip, 1.0 + clip) * advantage;

            //GRAV_LOG_LINE_INFO("Policy Ratio");
            //ratio.print();
            //GRAV_LOG_LINE_INFO("Unclamped Surrogate");
            //surr1.print();
            ////torch::print(surr1);
            ////GRAV_LOG_LINE_INFO("");
            //GRAV_LOG_LINE_INFO("Clamped Surrogate");
            //surr2.print();
            ////torch::print(surr2);
            ////GRAV_LOG_LINE_INFO("");

            // Claculate the actor and critic loss
            torch::Tensor actorLoss = -torch::min(surr1, surr2).mean();
            torch::Tensor criticLoss = (returns - value).pow(2).mean();

            //GRAV_LOG_LINE_INFO("Actor Loss");
            //torch::print(actorLoss);
            //GRAV_LOG_LINE_INFO("");
            //GRAV_LOG_LINE_INFO("Critic Loss");
            //torch::print(criticLoss);
            //GRAV_LOG_LINE_INFO("");

            // Calculate the overall loss
            torch::Tensor loss = 0.5 * criticLoss + actorLoss - 0.001 * entropy;

            //GRAV_LOG_LINE_INFO("Total Loss");
            //torch::print(loss.sum());
            //GRAV_LOG_LINE_INFO("");
            
            // Take a step through the optimization
            m_Optimizer.zero_grad();
            loss.sum().backward();
            m_Optimizer.step();
        }
    }
}

void GRAVEngine::AI::Training::Algorithms::PPO::saveModel(const std::string& filePath)
{
}
void GRAVEngine::AI::Training::Algorithms::PPO::loadModel(const std::string& filePath)
{
}

void GRAVEngine::AI::Training::Algorithms::PPO::addTrajectory(trajectory trajectory)
{
    // Create a buffer from the trajectory
    updateBuffer buffer = trajectory.toUpdateBuffer();

    // Set the buffer's return values
    buffer.m_Returns = gae(trajectory);

    // Detach the return values
    for (auto it = buffer.m_Returns.begin(); it != buffer.m_Returns.end(); it++)
        *it = it->detach();
    //// Detach the advantages values
    //for (auto it = buffer.m_Advantages.begin(); it != buffer.m_Advantages.end(); it++)
    //    it->detach();
    
    // Detach the log probs
    for (auto it = buffer.m_LogProbs.begin(); it != buffer.m_LogProbs.end(); it++)
    {
        // Detach the continuous log probs
        if (it->m_ContinuousActions.defined())
            it->m_ContinuousActions = it->m_ContinuousActions.detach();

        // Detach each discrete log prob
        for (auto dis = it->m_DiscreteActions.begin(); dis != it->m_DiscreteActions.end(); dis++)
            *dis = dis->detach();
    }

    // Detach the values
    for (auto it = trajectory.m_Experiences.begin(); it != trajectory.m_Experiences.end(); it++)
    {
        torch::Tensor& value = std::get<1>(it->m_ActionInformation);
        value = value.detach();
    }

    // Set the advantages
    for (size_t i = 0; i < buffer.m_Returns.size(); i++)
    {
        // Have to push back as the assignment operator will not work because the item doesn't exist yet
        buffer.m_Advantages.push_back(buffer.m_Returns[i] - std::get<1>(trajectory.m_Experiences[i].m_ActionInformation));
    }

    // Add to the update buffer the trajectory
    m_UpdateBuffer.append(buffer);
}

void GRAVEngine::AI::Training::Algorithms::PPO::sendToDevice(inferenceDevice device)
{
    switch (device)
    {
    case GRAVEngine::AI::inferenceDevice::CPU:
        m_Model->operator->()->to(torch::kCPU);
        break;
    case GRAVEngine::AI::inferenceDevice::GPU:
        // Send the model to the GPU if cuda core are actually available.
        m_Model->operator->()->to(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);
        break;
    default:
        break;
    }
}

std::vector<torch::Tensor> GRAVEngine::AI::Training::Algorithms::PPO::gae(trajectory trajectory)
{
    std::vector<torch::Tensor> returns;
    auto gamma = m_Hyperparameters->m_Gamma;
    auto tau = m_Hyperparameters->m_Lamda;

    torch::Tensor gae = torch::zeros({ 1 });
    // Loop backwards not including the last overall item from the trajectory. The last one is considered the terminal state
    for (int64 i = trajectory.size() - 1 - 1; i >= 0; i--)
    {
        torch::Tensor reward = trajectory.m_Experiences[i].m_Reward;
        torch::Tensor mask = (1 - trajectory.m_Experiences[i].m_Done);
        torch::Tensor nextValue = std::get<1>(trajectory.m_Experiences[i + 1].m_ActionInformation);
        torch::Tensor value = std::get<1>(trajectory.m_Experiences[i].m_ActionInformation);

        // Calculate the delta
        auto delta = reward + gamma * nextValue * mask - value;
        // Calculate gae
        gae = delta + gamma * tau * mask * gae;
        // Insert the gae value into the beginning of the returns as we are looping backwards
        returns.insert(returns.begin(), gae + value);
    }

    return returns;
}

//void GRAVEngine::AI::Training::Algorithms::PPO::initialize(networkSettings settings, ref<hyperparameters> parameters)
//{
//}
