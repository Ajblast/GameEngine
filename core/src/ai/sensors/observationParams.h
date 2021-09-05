#pragma once

#include "common.h"
#include "ai/tensors/tensorShape.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Sensors
		{
			// Paramaters about an observation
			struct observationParams
			{
			public:
				observationParams(size_t length);
				observationParams(GRAVEngine::AI::Tensors::tensorShape shape);

				// Number of dimensions of the observation
				inline const size_t rank() const { return m_Shape.length(); }

			public:
				// Observation shape
				GRAVEngine::AI::Tensors::tensorShape m_Shape;
			};
		}
	}
}