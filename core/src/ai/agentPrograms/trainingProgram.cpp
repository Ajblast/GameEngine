#include "gravpch.h"
#include "trainingProgram.h"
#include "ai/training/trainerSettings.h"
#include "ai/environmentManager.h"
#include "ai/sensors/bufferWriter.h"
#include "ai/agentEpisodeId.h"
#include <torch/torch.h>

GRAVEngine::AI::trainingProgram::trainingProgram(std::string name, const std::vector<ref<Sensors::ISensor>>& sensors, Actions::actuatorList& actuators) : m_Name(name)
{
	// Network Settings
	GRAVEngine::AI::Training::networkSettings settings;

	// Add all of the sensor informations
	for (auto it = sensors.begin(); it != sensors.end(); it++)
		settings.m_ObservationSpecs.push_back(it->operator->()->getObservationParams());

	// Get the action spec
	settings.m_ActionSpec = actuators.totalActionSpec();

	// Create the trainer settings
	GRAVEngine::AI::Training::trainerSettings trainerSettings;
	trainerSettings.m_ProgramName = name;
	trainerSettings.m_Algorithm = GRAVEngine::AI::Training::algorithmType::PPO;
	trainerSettings.m_NetworkSettings = std::move(settings);

	// Create the trainer for this AI if it doesn't exist
	if (GRAVEngine::AI::environmentManager::instance().controller().getTrainer(name) == nullptr)
		GRAVEngine::AI::environmentManager::instance().controller().createTrainer(trainerSettings);
}

void GRAVEngine::AI::trainingProgram::requestDecision(agentInfo info, scope<ref<Sensors::ISensor>[]>& sensors, size_t count)
{
	// Get the trainer
	auto trainer = GRAVEngine::AI::environmentManager::instance().controller().getTrainer(m_Name);

	m_AgentId = info.m_EpisodeID;

	std::vector<torch::Tensor> observations;
	for (size_t i = 0; i < count; i++)
	{
		auto sensor = sensors[i];
		int64 sensorSize = sensor->getObservationParams().m_Shape.length();

		// Create a place to store the sensor data
		scope<float[]> sensorData = createScope<float[]>(sensorSize);

		// Create the writer
		Sensors::bufferWriter writer = Sensors::bufferWriter(sensorData.get(), sensorSize);

		// Write the sensor data
		sensor->write(writer);

		// Create a tensor
		torch::Tensor sensorTensor = torch::from_blob(sensorData.get(), { sensorSize }).clone().unsqueeze(0);

		// Add this sensor's data to the list of data
		observations.push_back(sensorTensor);
	}

	if (trainer == nullptr)
		return;

	// Add the observation to the trainer
	trainer->addObservation(info.m_EpisodeID, info, observations);
}

GRAVEngine::AI::Actions::actionBuffer GRAVEngine::AI::trainingProgram::decideAction()
{
	// Get the trainer
	auto trainer = GRAVEngine::AI::environmentManager::instance().controller().getTrainer(m_Name);

	// Return null buffer if the trainer doesn't exist
	if (trainer == nullptr)
		return Actions::actionBuffer();

	return trainer->getActions(m_AgentId);
}

void GRAVEngine::AI::trainingProgram::sendToDevice(inferenceDevice device)
{
	// Get the trainer
	auto trainer = GRAVEngine::AI::environmentManager::instance().controller().getTrainer(m_Name);

	// Return null buffer if the trainer doesn't exist
	if (trainer == nullptr)
		return;

	// Tell the trainer to be sent to a deice
	trainer->sendToDevice(device);
}
