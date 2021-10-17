#include "gravpch.h"
#include "trainerController.h"
#include "exceptions/standard/invalidArgumentException.h"
#include "io/files/io.h"

GRAVEngine::AI::Training::trainerController::trainerController(scope<algorithmFactory> algorithmFactory) : m_AlgorithmFactory(std::move(algorithmFactory))
{
	if (m_AlgorithmFactory == nullptr)
		throw Exceptions::invalidArgumentException("Algorithm Factory was null");
}

GRAVEngine::AI::Training::trainerController::~trainerController()
{
	Locks::scopedLock<decltype(m_TrainerLock)> lock(m_TrainerLock);

	// Trainers should save before exiting
	for (auto it = m_Trainers.begin(); it != m_Trainers.end(); it++)
		it->second->save();
}

const GRAVEngine::AI::Training::algorithmFactory& GRAVEngine::AI::Training::trainerController::getAlgorithmFactory() const
{
	// Return the current algorithm factory
	return *m_AlgorithmFactory.get();
}
const GRAVEngine::ref<GRAVEngine::AI::Training::trainer> GRAVEngine::AI::Training::trainerController::getTrainer(const std::string& programName)
{
	Locks::scopedLock<decltype(m_TrainerLock)> lock(m_TrainerLock);

	auto it = m_Trainers.find(programName);
	// Return a null pointer if the trainer doesn't exist
	if (it == m_Trainers.end())
		return nullptr;

	// Return the trainer
	return m_Trainers.at(programName);
}

void GRAVEngine::AI::Training::trainerController::createTrainer(trainerSettings settings)
{
	GRAV_PROFILE_FUNCTION();

	// Do nothing if the trainer already exists
	if (m_Trainers.find(settings.m_ProgramName) != m_Trainers.end())
	{
		GRAV_LOG_LINE_WARN("Attempting to create trainer for already existing program {%s}", settings.m_ProgramName.c_str());
		return;
	}

	{
		Locks::scopedLock<decltype(m_TrainerLock)> lock(m_TrainerLock);

		// Create the algorithm
		scope<ITrainingAlgorithm> algorithm = m_AlgorithmFactory->createAlgorithm(settings);

		if (IO::exists(settings.m_StatsFolder + "\\" + settings.m_ProgramName + "Model.pt"))
			algorithm->loadModel(settings.m_StatsFolder + "\\" + settings.m_ProgramName);

		// Create the trainer
		ref<trainer> t = createRef<trainer>(settings, std::move(algorithm));

		// Add the trainer
		m_Trainers.emplace(settings.m_ProgramName, t); 
	}
}
void GRAVEngine::AI::Training::trainerController::createTrainer(const std::string& programName, const std::string& folderPath, scope<ITrainingAlgorithm> algorithm)
{
	GRAV_PROFILE_FUNCTION();

	// Check if the algorithm exists
	if (algorithm == nullptr)
		throw Exceptions::invalidArgumentException("Algorithm is null.");

	// Do nothing if the trainer already exists
	if (m_Trainers.find(programName) != m_Trainers.end())
	{
		GRAV_LOG_LINE_WARN("Attempting to create trainer for already existing program {%s}", programName.c_str());
		return;
	}

	{
		Locks::scopedLock<decltype(m_TrainerLock)> lock(m_TrainerLock);

		// Create the trainer settings based on the algorithm
		trainerSettings settings = { programName, algorithm->getAlgorithmType(), algorithm->getNetworkSettings(), 64, folderPath, 1000 };

		// Create the algorithm
		scope<ITrainingAlgorithm> algorithm = m_AlgorithmFactory->createAlgorithm(settings);

		if (IO::exists(settings.m_StatsFolder + "\\" + settings.m_ProgramName + "Model.pt"))
			algorithm->loadModel(settings.m_StatsFolder + "\\" + settings.m_ProgramName);

		// Create the trainer
		ref<trainer> t = createRef<trainer>(settings, std::move(algorithm));

		// Add the trainer
		m_Trainers.emplace(programName, t);
	}
}

void GRAVEngine::AI::Training::trainerController::step()
{
	Locks::scopedLock<decltype(m_TrainerLock)> lock(m_TrainerLock);

	GRAV_PROFILE_FUNCTION();

	for (auto it = m_Trainers.begin(); it != m_Trainers.end(); it++)
		it->second->step();
}
