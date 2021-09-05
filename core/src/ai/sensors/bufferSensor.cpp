#include "gravpch.h"
#include "bufferSensor.h"

GRAVEngine::AI::Sensors::bufferSensor::bufferSensor(size_t obsCapacity, size_t obsSize, std::string name) :
	m_Capacity(obsCapacity), m_ObsSize(obsSize), m_Name(m_Name), m_Count(0), m_Buffer(createScope<float[]>(obsCapacity * m_ObsSize)), m_Params(obsSize)
{
}

void GRAVEngine::AI::Sensors::bufferSensor::addObservation(float* obs, size_t size)
{
	GRAV_ASSERT(size == m_ObsSize);

	// Don't add the observation if buffer is full or over capacity
	if (m_Count >= m_Capacity)
		return;

	// Copoy the data offset from the current count
	memcpy(m_Buffer.get() + m_Count * m_ObsSize, obs, m_ObsSize * sizeof(float));

	m_Count++;
}

size_t GRAVEngine::AI::Sensors::bufferSensor::write(observationWriter& writer)
{
	writer.write(m_Buffer.get(), m_ObsSize * m_Capacity);

	return m_ObsSize * m_Capacity;
}

void GRAVEngine::AI::Sensors::bufferSensor::update()
{
	// Reset the buffer on update;
	reset();
}
void GRAVEngine::AI::Sensors::bufferSensor::reset()
{
	// Reset the information;
	m_Count = 0;
	memset(m_Buffer.get(), 0, m_Capacity * m_ObsSize * sizeof(float));
}

GRAVEngine::AI::Sensors::observationParams GRAVEngine::AI::Sensors::bufferSensor::getObservationParams()
{
	return m_Params;
}

std::string GRAVEngine::AI::Sensors::bufferSensor::getName()
{
	return m_Name;
}
