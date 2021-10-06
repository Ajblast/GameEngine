#pragma once

#include "common.h"
#include "observationParams.h"
#include "observationWriter.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Sensors
		{
			// Abstract sensor
			class GRAVAPI ISensor
			{
			public:
				ISensor() = default;
				virtual ~ISensor() = default;

				virtual size_t write(observationWriter& writer) = 0;
				virtual void update() = 0;
				virtual void reset() = 0;
				virtual observationParams getObservationParams() = 0;

				// Used for sorting for determinism
				virtual std::string getName() = 0;
			};
		}
	}
}