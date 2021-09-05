#pragma once

#include "ai/training/hyperparameters.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			namespace Algorithms
			{
				// Hyperparameters for the PPO algorithm
				struct ppoHyperparameters : public hyperparameters
				{
				public:
					ppoHyperparameters() = default;
					virtual ~ppoHyperparameters() = default;

				public:
					size_t m_EpochCount = 3;		// 3-10:		Number of passes through experience buffer.
					float m_Epsilon = 0.3f;			// 0.1-0.3:		How rapidly policy can evolve during training
					float m_Lamda = 0.95f;			// 0.9-0.95:	GAE regulariation value for learning rate
					float m_Beta = 5.03e-3f;		// 1e-4-1e-2:	Entropy regularization strength
					float m_Clip = 0.2f;			// 0.1-0.3:		How much should the surrogate be clipped
				};
			}
		}
	}
}