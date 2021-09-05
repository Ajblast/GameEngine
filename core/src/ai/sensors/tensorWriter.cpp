#include "gravpch.h"
#include "tensorWriter.h"

GRAVEngine::AI::Sensors::tensorWriter::tensorWriter(Tensors::tensor tensor) : observationWriter(observationParams(tensor.shape())), m_Tensor(std::move(tensor))
{
}

void GRAVEngine::AI::Sensors::tensorWriter::write(float* data, size_t count)
{
	// The amount of data to write needs to be the same shape as the tensor itself
	GRAV_ASSERT(count <= m_Params.m_Shape.length());

	// Copy all of the data into the tensor
	for (size_t i = 0; i < count; i++)
	{
		m_Tensor.set(data[i], 1, i);
	}
}
