#pragma once

#include "common.h"
#include "trainer.h"
#include "algorithmFactory.h"
#include "ITrainingAlgorithm.h"
#include "trainerSettings.h"
#include <string>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			class trainerController
			{
			public:
				trainerController(scope<algorithmFactory> algorithmFactory);
			public:
				const algorithmFactory& getAlgorithmFactory() const;
				const ref<trainer>& getTrainer(const std::string& programName) const;

				// Create a trainer with the desired settings
				void createTrainer(trainerSettings settings);
				// Create a trainer for the program and the desired algorithm. Takes ownership of the algorithm
				void createTrainer(const std::string& programName, scope<ITrainingAlgorithm> algorithm);

				// Step through every trainer
				void step();
			private:
				std::map<std::string, ref<trainer>> m_Trainers;	// List of trainer
				scope<algorithmFactory> m_AlgorithmFactory;		// The algorithm factory for creating trainers
			};
		}
	}
}