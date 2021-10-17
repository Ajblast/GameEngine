#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			// Hyperparameters for all training algorithms
			struct GRAVAPI hyperparameters
			{
			public:
				hyperparameters() = default;
				virtual ~hyperparameters() = default;

			public:
				size_t m_MaxSteps = 5000000;	// Maximum number of steps
				size_t m_BatchSize = 128;		// Size of a batch
				size_t m_BufferSize = 4096;		// Size the update buffer needs to be before training will occur
				size_t trajectoryHorizon = 64;	// How many observations before a trajectory is filled and should be cut off
				float m_Gamma = .99f;			// How much future opbservations should influence compared to recent observations
				float m_LearningRate = 3e-4f;	// Learning rate
			};
		}
	}
}