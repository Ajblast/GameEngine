#pragma once

#include "common.h"
#include "ISensor.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Sensors
		{
			// Sensor that uses an internal buffer to hold multiple observations
			class GRAVAPI bufferSensor : public ISensor
			{
			public:
				bufferSensor(size_t obsCapacity, size_t obsSize, std::string name);
				void addObservation(float* obs, size_t size);

				// Inherited via ISensor
				virtual size_t write(observationWriter& writer) override;
				virtual void update() override;
				virtual void reset() override;
				virtual observationParams getObservationParams() override;
				virtual std::string getName();
			private:
				std::string m_Name;			// Name of the sensor
				scope<float[]> m_Buffer;	// The buffer
				size_t m_Capacity;			// Capacity of the buffer
				size_t m_Count;				// Current number of observations
				size_t m_ObsSize;			// Size of each observation
				observationParams m_Params;	// Observation parameters
			};
		}
	}
}