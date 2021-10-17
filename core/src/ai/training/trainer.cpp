#include "gravpch.h"
#include "trainer.h"
#include "exceptions/standard/invalidArgumentException.h"
#include "trajectory.h"
#include <mutex>

GRAVEngine::AI::Training::trainer::trainer(trainerSettings settings, scope<ITrainingAlgorithm> algorithm) : 
	m_Settings(settings), m_Algorithm(std::move(algorithm)), m_Stats(), m_CurrentStep(0), m_NextSummaryStep(0)
{
	m_Stats.createStat("Cumulative Reward");
	m_Stats.createStat("Actor Loss");
	m_Stats.createStat("Critic Loss");
	m_Stats.createStat("Total Loss");

	if (m_Algorithm == nullptr)
		throw Exceptions::invalidArgumentException("Algorithm is null.");
}
GRAVEngine::AI::Training::trainer& GRAVEngine::AI::Training::trainer::operator=(trainer&& other) noexcept
{
	if (this != &other)
	{
		//std::lock(m_SpinLock, other.m_SpinLock);
		std::scoped_lock lock(m_SpinLock, other.m_SpinLock);

		m_Settings = other.m_Settings;
		m_Algorithm = std::move(other.m_Algorithm);

		m_DecidedAgentActions = std::move(other.m_DecidedAgentActions);
		m_Experiences = std::move(other.m_Experiences);

		m_LastObservation = std::move(other.m_LastObservation);
		m_CumulativeRewards = std::move(other.m_CumulativeRewards);
		m_Steps = std::move(other.m_Steps);

		m_Trajectories = std::move(other.m_Trajectories);
		m_Stats = std::move(other.m_Stats);

		m_CurrentStep = other.m_CurrentStep;
		m_NextSummaryStep = other.m_NextSummaryStep;
	}
	return *this;
}

void GRAVEngine::AI::Training::trainer::addObservation(agentEpisodeId agentId, agentInfo info, std::vector<torch::Tensor> observation)
{
	// Lock the trainer so the observation can be created
	Locks::scopedLock<decltype(m_SpinLock)> lock(m_SpinLock);

	GRAV_PROFILE_FUNCTION();

	// Is the previous observation a terminal step. This means that the current observation is not the result of a previous action
	bool terminal = info.m_IsDone;

	// Was the last observation stored. i.e., 
	bool storedLastObservation = m_LastObservation.find(agentId) != m_LastObservation.end();

	// Only run this if the last observation was stored. This is for the case in which the agent just starts up
	if (storedLastObservation)
	{
		// Get the components of the last observation
		std::vector<torch::Tensor> lastObservation = std::get<0>(m_LastObservation[agentId]);
		agentInfo lastInfo = std::get<1>(m_LastObservation[agentId]);
		Models::ActorCritic::actorCriticOputput lastActionInfo = std::get<2>(m_LastObservation[agentId]);

		// Create the agent experience of the last step. We can do this now because the results of the old step are passed in with the new observation
		agentExperience experience = agentExperience(
			lastObservation,				// Last observation taken to get the reward
			torch::tensor(info.m_Reward),	// The reward cause from the last observation and actions
			torch::tensor(terminal ? 1 : 0),// Did the last observation and actions result in a terminal state
			lastActionInfo					// Information about the previous action. Actions, LogProbs, Entropy
		);

		// Add the experience for the agent
		m_Experiences[agentId].push_back(experience);

		// Increase the number of steps the agent has taken if it is not the terminal state
		if (terminal == false)
			m_Steps[agentId]++;
		// Increase the cumulative reward
		m_CumulativeRewards[agentId] += info.m_Reward;


		// Create a trajectory if the number of experiences is larger than the trajectory horizon or if the last observation was terminal
		if (m_Experiences[agentId].size() >= m_Settings.m_TrajectoryHorizon || terminal)
		{
			// Create the trajectory by copying the experiences
			trajectory traj = trajectory(m_Experiences[agentId]);

			// Empty the experience buffer after the trajectory is made
			m_Experiences.erase(agentId);

			// Add the trajectory to the list of trajectories for when the update is called on the model
			m_Trajectories.push_back(traj);

			// Check if the cumulative reward should be added before checking if a write should happen
			if (terminal)
				m_Stats.addStat("Cumulative Reward", m_CumulativeRewards[agentId]);

			// Increate the total step
			m_CurrentStep += traj.size();
			// Should the model be saved because of the step
			shouldWriteSummary(m_CurrentStep + traj.size());
		}


		// If the last state was terminal, record the episode length and clear experience data associated with the agent
		if (terminal)
		{
			m_Experiences.erase(agentId);		// Clear the experience buffer
			m_CumulativeRewards.erase(agentId);	// Clear the cumulative reward as a terminal state was found
			m_Steps.erase(agentId);				// Clear the number of steps the agent took
		}
	}

	// This will assume that the observation will already have batch information inside of it.
	Models::ActorCritic::actorCriticOputput modelOutput = m_Algorithm->forward(observation);
	auto calculatedActions = std::get<0>(std::get<0>(modelOutput));

	// Take the calculated actions and store them
	m_DecidedAgentActions[agentId] = calculatedActions.toAgentBuffer();

	// Store the current observation as the previous observation because we are always working in the past
	m_LastObservation[agentId] = { observation, info, modelOutput };
	
}
void GRAVEngine::AI::Training::trainer::step()
{
	Locks::scopedLock<decltype(m_SpinLock)> lock(m_SpinLock);

	GRAV_PROFILE_FUNCTION();

	// Add all of the trajectories to the algorithm
	for (auto it = m_Trajectories.begin(); it != m_Trajectories.end(); it++)
		m_Algorithm->addTrajectory(*it);

	// Clear the trajectories buffer because they were all added
	m_Trajectories.clear();

	if (m_Algorithm->shouldUpdate())
	{
		// Get the loss from the update
		updateLoss loss = m_Algorithm->update();
		m_Stats.addStat("Actor Loss", loss.m_ActorLoss);
		m_Stats.addStat("Critic Loss", loss.m_CriticLoss);
		m_Stats.addStat("Total Loss", loss.m_TotalLoss);
	}
}
void GRAVEngine::AI::Training::trainer::reset()
{
	// Lock the trainer so everything can be removed
	Locks::scopedLock<decltype(m_SpinLock)> lock(m_SpinLock);

	GRAV_PROFILE_FUNCTION();

	m_DecidedAgentActions.clear();
	m_Experiences.clear();

	m_LastObservation.clear();
	m_CumulativeRewards.clear();
	m_Steps.clear();

	m_Trajectories.clear();
}

const GRAVEngine::AI::Actions::actionBuffer GRAVEngine::AI::Training::trainer::getActions(agentEpisodeId agentId)
{
	// Lock the trainer so the observation can be created
	Locks::scopedLock<decltype(m_SpinLock)> lock(m_SpinLock);

	// Return an empty buffer if the agent is not in the map
	if (m_DecidedAgentActions.find(agentId) == m_DecidedAgentActions.end())
		return Actions::actionBuffer();

	return m_DecidedAgentActions[agentId];
}
void GRAVEngine::AI::Training::trainer::printModel() const
{
	m_Algorithm->print();
}
void GRAVEngine::AI::Training::trainer::save()
{
	GRAV_PROFILE_FUNCTION();

	// Write out the model
	m_Algorithm->saveModel(m_Settings.m_StatsFolder + "\\" + m_Settings.m_ProgramName);

	// Save the statistics
	m_Stats.write(m_Settings.m_StatsFolder + "\\" + m_Settings.m_ProgramName + ".csv");
}
void GRAVEngine::AI::Training::trainer::sendToDevice(inferenceDevice device) const
{
	// Send the algorithm to a device
	m_Algorithm->sendToDevice(device);
}

void GRAVEngine::AI::Training::trainer::shouldWriteSummary(size_t interval)
{
	if (m_NextSummaryStep == 0)
		m_NextSummaryStep = m_CurrentStep + (m_Settings.m_SummaryFrequency - m_CurrentStep % m_Settings.m_SummaryFrequency);
	else if (interval >= m_NextSummaryStep && m_CurrentStep != 0)
	{
		m_NextSummaryStep = m_CurrentStep + (m_Settings.m_SummaryFrequency - m_CurrentStep % m_Settings.m_SummaryFrequency);

		save();
	}


}
