#include "gravpch.h"
#include "trainerController.h"
#include "exceptions/invalidArgumentException.h"

GRAVEngine::AI::Training::trainerController::trainerController(scope<algorithmFactory> algorithmFactory) : m_AlgorithmFactory(std::move(algorithmFactory))
{
	if (m_AlgorithmFactory == nullptr)
		throw Exceptions::invalidArgumentException("Algorithm Factory was null");
}

const GRAVEngine::AI::Training::algorithmFactory& GRAVEngine::AI::Training::trainerController::getAlgorithmFactory() const
{
	// Return the current algorithm factory
	return *m_AlgorithmFactory.get();
}
const GRAVEngine::ref<GRAVEngine::AI::Training::trainer>& GRAVEngine::AI::Training::trainerController::getTrainer(const std::string& programName) const
{
	auto it = m_Trainers.find(programName);
	// Return a null pointer if the trainer doesn't exist
	if (it == m_Trainers.end())
		return nullptr;

	// Return the trainer
	return m_Trainers.at(programName);
}

void GRAVEngine::AI::Training::trainerController::createTrainer(trainerSettings settings)
{
	// Do nothing if the trainer already exists
	if (m_Trainers.find(settings.m_ProgramName) != m_Trainers.end())
	{
		GRAV_LOG_LINE_WARN("Attempting to create trainer for already existing program {%s}", settings.m_ProgramName.c_str());
		return;
	}

	// Create the algorithm
	scope<ITrainingAlgorithm> algorithm = m_AlgorithmFactory->createAlgorithm(settings);

	// Create the trainer
	ref<trainer> t = createRef<trainer>(settings, std::move(algorithm));

	// Add the trainer
	m_Trainers.emplace(settings.m_ProgramName, t);
}
void GRAVEngine::AI::Training::trainerController::createTrainer(const std::string& programName, scope<ITrainingAlgorithm> algorithm)
{
	// Check if the algorithm exists
	if (algorithm == nullptr)
		throw Exceptions::invalidArgumentException("Algorithm is null.");

	// Do nothing if the trainer already exists
	if (m_Trainers.find(programName) != m_Trainers.end())
	{
		GRAV_LOG_LINE_WARN("Attempting to create trainer for already existing program {%s}", programName.c_str());
		return;
	}

	// Create the trainer settings based on the algorithm
	trainerSettings settings = { programName, algorithm->getAlgorithmType(), algorithm->getNetworkSettings(), 64 };

	// Create the trainer
	ref<trainer> t = createRef<trainer>(settings, std::move(algorithm));

	// Add the trainer
	m_Trainers.emplace(programName, t);
}
