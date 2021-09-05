#pragma once

#include "common.h"
#include "algorithmFactory.h"
#include "algorithms/PPO.h"
#include "algorithms/ppoHyperparameters.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			// Default algorithm factory.
			class defaultFactory : public algorithmFactory
			{
			public:
				defaultFactory() = default;
				virtual ~defaultFactory() = default;

				// Create an algorithm with the desired settings
				virtual scope<ITrainingAlgorithm> createAlgorithm(trainerSettings settings) const override;
				// Create the hyperparameters for the algorithm type
				virtual ref<hyperparameters> createHyperparameters(algorithmType type) const override;
			};
		}
	}
}