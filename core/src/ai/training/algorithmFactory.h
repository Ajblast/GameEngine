#pragma once

#include "common.h"
#include "ITrainingAlgorithm.h"
#include "algorithmType.h"
#include "hyperparameters.h"
#include "trainerSettings.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			// Factory to create algorithms and hyperparameters for a training algorithm
			class algorithmFactory
			{
			public:
				algorithmFactory() = default;
				virtual ~algorithmFactory() = default;

			public:
				// Create an algorithm with the desired settings
				virtual scope<ITrainingAlgorithm> createAlgorithm(trainerSettings settings) const = 0;
				// Create the hyperparameters for the algorithm type
				virtual ref<hyperparameters> createHyperparameters(algorithmType type) const = 0;
			};
		}
	}
}