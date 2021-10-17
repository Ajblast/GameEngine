#include "gravpch.h"
#include "PPO.h"
#include "../../models/model.h"

GRAVEngine::AI::Training::Algorithms::PPO::PPO(networkSettings settings, ref<ppoHyperparameters> hyperparameters) :
    m_Model(std::move(Models::ActorCritic::actorCritic(settings))),
    m_Hyperparameters(hyperparameters),
    m_Optimizer(m_Model->parameters(), torch::optim::AdamOptions(0.0001).betas({ 0.5, 0.999 }))
{
    GRAV_ASSERT(hyperparameters != nullptr);    // Make sure that the hyperparameters are actually passed in
}

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


GRAVEngine::AI::Models::ActorCritic::actorCriticOputput GRAVEngine::AI::Training::Algorithms::PPO::forward(std::vector<torch::Tensor> inputs)
{
    return m_Model->forward(inputs);
}


void GRAVEngine::AI::Training::Algorithms::PPO::print() const
{
    std::stringstream ss;

    ss << m_Model << std::endl;

    GRAV_LOG_LINE_INFO("\nPrint Model:\n%s", ss.str().c_str());

}
GRAVEngine::AI::Training::updateLoss GRAVEngine::AI::Training::Algorithms::PPO::update()
{
    GRAV_PROFILE_FUNCTION();
    
    updateLoss loss{ 0, 0, 0 };

    // Check if there are enough update experiences to be able to update
    if (shouldUpdate() == false)
        return loss;

    size_t batchSize = m_UpdateBuffer.size();               // Total batch size
    size_t miniBatchSize = m_Hyperparameters->m_BatchSize;  // Size of the minibatches
    float clip = m_Hyperparameters->m_Clip;                 // Clipping parameter

    // Training stats
    std::vector<float> actorLosses;
    std::vector<float> criticLosses;
    std::vector<float> totalLosses;


    // Normalize the advantages to be in a normal distribution
    torch::Tensor advantages = torch::cat(m_UpdateBuffer.m_Advantages);
    for (auto it = m_UpdateBuffer.m_Advantages.begin(); it != m_UpdateBuffer.m_Advantages.end(); it++)
        *it = (*it - advantages.mean()) / (advantages.std() + 1e-10);

    // Run for the specified epoch count
    for (size_t i = 0; i < m_Hyperparameters->m_EpochCount; i++)
    {
        // For as many minibatches that can be created
        for (size_t j = 0; j < batchSize / miniBatchSize; j++)
        {
            GRAV_PROFILE_SCOPE("Update Batch");

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
            if (advantage.isnan().any().item<bool>() || advantage.isinf().any().item<bool>())
            {
                GRAV_LOG_LINE_WARN("advantage is nan or inf");
                std::cout << advantage << std::endl;
            }
            if (returns.isnan().any().item<bool>() || returns.isinf().any().item<bool>())
            {
                GRAV_LOG_LINE_WARN("returns is nan or inf");
                std::cout << returns << std::endl;
            }

            //GRAV_LOG_LINE_INFO("Advantage");
            //advantage.print();
            //GRAV_LOG_LINE_INFO("Returns");
            //returns.print();

            Models::ActorCritic::actorCriticOputput output;
            {
                GRAV_PROFILE_SCOPE("Run Model");
                // Run through the model
                output = m_Model->forward(observation);
            }
            torch::Tensor value = std::get<1>(output);
            torch::Tensor entropy = std::get<2>(std::get<0>(output));
            if (value.isnan().any().item<bool>() || value.isinf().any().item<bool>())
            {
                GRAV_LOG_LINE_WARN("value is nan or inf");
                std::cout << value << std::endl;
            }
            if (entropy.isnan().any().item<bool>() || entropy.isinf().any().item<bool>())
            {
                GRAV_LOG_LINE_WARN("entropy is nan or inf");
                std::cout << entropy << std::endl;
            }


            //GRAV_LOG_LINE_INFO("Critic Value");
            //value.print();
            //GRAV_LOG_LINE_INFO("Distribution Entropy");
            //torch::print(entropy);
            //GRAV_LOG_LINE_INFO("");

            Models::ActorCritic::agentLogProbs newLogProbs = m_Model->logProbs(observation, actions);


            // Calculate the surrogates
            torch::Tensor flattenedNewLog = newLogProbs.flatten();
            torch::Tensor flattenedOldLog = oldlogProbs.flatten();
            if (flattenedNewLog.isnan().any().item<bool>() || flattenedNewLog.isinf().any().item<bool>())
            {
                GRAV_LOG_LINE_WARN("flattenedNewLog is nan or inf");
                std::cout << flattenedNewLog << std::endl;
            }
            if (flattenedOldLog.isnan().any().item<bool>() || flattenedOldLog.isinf().any().item<bool>())
            {
                GRAV_LOG_LINE_WARN("flattenedOldLog is nan or inf");
                std::cout << flattenedOldLog << std::endl;
            }


            torch::Tensor ratio = (flattenedNewLog - flattenedOldLog).exp();
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
            if (actorLoss.isnan().any().item<bool>() || actorLoss.isinf().any().item<bool>())
            {
                GRAV_LOG_LINE_WARN("actorLoss is nan or inf");
                std::cout << actorLoss << std::endl;
            }
            if (criticLoss.isnan().any().item<bool>() || criticLoss.isinf().any().item<bool>())
            {
                GRAV_LOG_LINE_WARN("criticLoss is nan or inf");
                std::cout << criticLoss << std::endl;
            }

            //GRAV_LOG_LINE_INFO("Actor Loss");
            //torch::print(actorLoss);
            //GRAV_LOG_LINE_INFO("");
            //GRAV_LOG_LINE_INFO("Critic Loss");
            //torch::print(criticLoss);
            //GRAV_LOG_LINE_INFO("");

            // Add the loss items per batch
            actorLosses.push_back(torch::abs(actorLoss).item<float>());
            criticLosses.push_back(criticLoss.item<float>());

            // Calculate the overall loss
            torch::Tensor loss = 0.5 * criticLoss + actorLoss - 0.001 * entropy.mean();
            
            if (loss.isnan().any().item<bool>() || loss.isinf().any().item<bool>())
            {
                GRAV_LOG_LINE_WARN("Loss is nan or inf");
                std::cout << loss << std::endl;
            }

            // Take a step through the optimization
            {
                GRAV_PROFILE_SCOPE("Optimize Tensor");
                m_Optimizer.zero_grad();
                totalLosses.push_back(loss.item<float>());

                //GRAV_LOG_LINE_INFO("Total Loss");
                //torch::print(lossSum);
                //GRAV_LOG_LINE_INFO("");

                if (loss.isnan().any().item<bool>() || loss.isinf().any().item<bool>())
                {
                    GRAV_LOG_LINE_WARN("Loss is nan or inf");
                    std::cout << loss << std::endl;
                }

                loss.backward();
                //torch::nn::utils::clip_grad_value_(m_Model->parameters(), 1.0);
                m_Optimizer.step();
            }

            if (loss.isnan().any().item<bool>() || loss.isinf().any().item<bool>())
            {
                GRAV_LOG_LINE_WARN("Loss is nan or inf");
                std::cout << loss << std::endl;
            }

        }
    }

    // Get the average loss from all of the batches
    loss = {
        torch::tensor(actorLosses).mean().item<float>(),
        torch::tensor(criticLosses).mean().item<float>(),
        torch::tensor(totalLosses).mean().item<float>()
    };

    // Clear the update buffer now that learning has happened from the experiences
    m_UpdateBuffer.clear();

    // Return the loss of every batch
    return loss;
}

void GRAVEngine::AI::Training::Algorithms::PPO::saveModel(const std::string& filePath)
{
    // Save the model and optimizer
    GRAVEngine::AI::Models::save(m_Model, m_Optimizer, filePath);
}
void GRAVEngine::AI::Training::Algorithms::PPO::loadModel(const std::string& filePath)
{
    // Load the model and optimizer
    GRAVEngine::AI::Models::load(m_Model, m_Optimizer, filePath);
}

void GRAVEngine::AI::Training::Algorithms::PPO::addTrajectory(trajectory trajectory)
{
    GRAV_PROFILE_FUNCTION();
    
    // Create a buffer from the trajectory
    updateBuffer buffer = trajectory.toUpdateBuffer();

    // Set the buffer's return values
    buffer.m_Returns = gae(trajectory);

    // Detach the return values
    for (auto it = buffer.m_Returns.begin(); it != buffer.m_Returns.end(); it++)
        *it = it->detach();
    
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
        m_Model->to(torch::kCPU);
        break;
    case GRAVEngine::AI::inferenceDevice::GPU:
        // Send the model to the GPU if cuda core are actually available.
        m_Model->to(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);
        break;
    default:
        break;
    }
}
std::vector<torch::Tensor> GRAVEngine::AI::Training::Algorithms::PPO::gae(trajectory trajectory)
{
    GRAV_PROFILE_FUNCTION();
    
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
