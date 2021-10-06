#pragma once

#include "common.h"
#include "ISensor.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Sensors
		{
			// Sensor with dynamic vector for storing observations
			class GRAVAPI vectorSensor : public ISensor
			{
			public:
				vectorSensor(uint32 obsSize, std::string name = "");
				virtual ~vectorSensor();

			public:
				// Write the observations to the writer
				virtual size_t write(observationWriter& writer) override;
				// Update the vector sensor
				virtual void update() override;
				// Reset the vector sensor
				virtual void reset() override;
				// Get the observation parameters
				virtual observationParams getObservationParams() override;
				// Get the name of this vector sensor
				virtual std::string getName() override;

				// Add an observation to the buffer
				void addObservation(float obs);
			private:
				void clear();
			private:
				std::string m_Name;						// Name of the sensor
				observationParams m_ObservationParams;	// Parameters about the observations
				scope<float[]> m_Values;				// Buffer containing all of the values

				size_t m_CurrentIndex;					// Current index into the values buffer
			};
		}
	}
}