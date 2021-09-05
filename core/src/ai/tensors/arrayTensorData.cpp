#include "gravpch.h"
#include "arrayTensorData.h"

GRAVEngine::AI::Tensors::arrayTensorData::arrayTensorData(size_t count) : m_Shape(count)
{
    // Create array for the data
    m_Data = createScope<float[]>(count);
}
GRAVEngine::AI::Tensors::arrayTensorData::arrayTensorData(tensorShape shape) : m_Shape(shape.length())
{
    // Create array for the data
    m_Data = createScope<float[]>(shape.length());
}
GRAVEngine::AI::Tensors::arrayTensorData::~arrayTensorData()
{
}

void GRAVEngine::AI::Tensors::arrayTensorData::reserve(size_t count)
{
    // Create array for the data
    m_Data = createScope<float[]>(count);
}

void GRAVEngine::AI::Tensors::arrayTensorData::upload(float* data, tensorShape shape)
{
    // Copy the data from the original shape into this shape. Ignore extra data
    for (size_t i = 0; i < shape.length() && i < capacity(); i++)
    {
        m_Data[i] = data[i];
    }
}

void GRAVEngine::AI::Tensors::arrayTensorData::download(float*& data, tensorShape shape)
{
    // Copy the data from the original shape into this shape. Ignore extra data
    for (size_t i = 0; i < shape.length() && i < capacity(); i++)
    {
        data[i] = m_Data[i];
    }
}

const size_t GRAVEngine::AI::Tensors::arrayTensorData::capacity() const
{
    return m_Shape;
}
