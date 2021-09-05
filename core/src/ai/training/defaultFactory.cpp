#include "gravpch.h"
#include "defaultFactory.h"

GRAVEngine::scope<GRAVEngine::AI::Training::ITrainingAlgorithm> GRAVEngine::AI::Training::defaultFactory::createAlgorithm(trainerSettings settings) const
{
	scope<ITrainingAlgorithm> retVal = nullptr;

	switch (settings.m_Algorithm)
	{
	case algorithmType::PPO:
	{
		// Create a unique pointer to the algorithm
		retVal = createScope<Algorithms::PPO>(
			settings.m_NetworkSettings,
			std::dynamic_pointer_cast<Algorithms::ppoHyperparameters>(createHyperparameters(settings.m_Algorithm)));
		break;
	}
	default:
		GRAV_LOG_ERROR("Unknown algorithm type");

		// Unknown algorithm type
		retVal = nullptr;
		break;
	}

	return retVal;
}

GRAVEngine::ref<GRAVEngine::AI::Training::hyperparameters> GRAVEngine::AI::Training::defaultFactory::createHyperparameters(algorithmType type) const
{
	ref<hyperparameters> retVal = nullptr;

	switch (type)
	{
	case algorithmType::PPO:
	{
		retVal = createRef<Algorithms::ppoHyperparameters>();
		break;
	}
	default:
		GRAV_LOG_ERROR("Unknown algorithm type");

		// Unknown algorithm type
		retVal = nullptr;
		break;
	}

	return retVal;
}
