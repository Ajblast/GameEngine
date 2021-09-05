#include "gravpch.h"
#include "bufferWriter.h"

GRAVEngine::AI::Sensors::bufferWriter::bufferWriter(float* data, size_t size) :observationWriter(observationParams(size)), m_Data(data), m_Size(size)
{
}

void GRAVEngine::AI::Sensors::bufferWriter::write(float* data, size_t count)
{
	GRAV_ASSERT(count <= m_Size);

	// Copy the data from the array into another
	memcpy(m_Data, data, count * sizeof(float));
}
