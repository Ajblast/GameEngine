#include "gravpch.h"
#include "tensor.h"

GRAVEngine::AI::Tensors::tensor::tensor(tensorShape shape, std::string name) : m_Shape(shape), m_Name(name)
{ 
	m_Data->reserve(shape.length());
}
GRAVEngine::AI::Tensors::tensor::tensor(tensorShape shape, float* data, std::string name) : m_Shape(shape), m_Name(name)
{
	m_Data->upload(data, shape);
}
GRAVEngine::AI::Tensors::tensor::tensor(tensorShape shape, scope<ITensorData>& data, std::string name) : m_Shape(shape), m_Name(name)
{
	tensorShape tempShape = tensorShape(data->capacity());
	float* temp = new float[data->capacity()];
	
	// Download the data
	data->download(temp, tempShape);

	// Upload into the tensor
	m_Data->upload(temp, tempShape);
	delete[] temp;
}
GRAVEngine::AI::Tensors::tensor::~tensor()
{
	 
}
GRAVEngine::AI::Tensors::tensor::tensor(tensor&& other) noexcept : m_Name(std::move(other.m_Name)), m_Shape(other.m_Shape), m_Data(std::move(other.m_Data))
{
}

GRAVEngine::AI::Tensors::tensor GRAVEngine::AI::Tensors::tensor::flatten()
{
	return tensor(m_Shape.flatten(), m_Data, m_Name);
}
GRAVEngine::AI::Tensors::tensor GRAVEngine::AI::Tensors::tensor::reshape(tensorShape shape)
{
	GRAV_ASSERT(shape.length() == m_Shape.length());
	return tensor(shape, m_Data, m_Name);
}

void GRAVEngine::AI::Tensors::tensor::set(float value, size_t b, size_t c)
{
	// Create a cache
	float* cache = new float[m_Shape.length()];

	// Get the data
	m_Data->download(cache, m_Shape);

	// Set the value into the cache
	cache[m_Shape.index(b, c)] = value;

	// Upload the data
	m_Data->upload(cache, m_Shape);

	delete[] cache;
}
void GRAVEngine::AI::Tensors::tensor::set(float value, size_t b, size_t h, size_t w, size_t c)
{
	// Create a cache
	float* cache = new float[m_Shape.length()];

	// Get the data
	m_Data->download(cache, m_Shape);

	// Set the value into the cache
	cache[m_Shape.index(b, h, w, c)] = value;

	// Upload the data
	m_Data->upload(cache, m_Shape);

	delete[] cache;

}

float GRAVEngine::AI::Tensors::tensor::at(size_t b, size_t c)
{
	// Create a cache
	float* cache = new float[m_Shape.length()];

	// Get the data
	m_Data->download(cache, m_Shape);

	float retVal = cache[m_Shape.index(b, c)];

	delete[] cache;
	return retVal;
}
float GRAVEngine::AI::Tensors::tensor::at(size_t b, size_t h, size_t w, size_t c)
{
	// Create a cache
	float* cache = new float[m_Shape.length()];

	// Get the data
	m_Data->download(cache, m_Shape);

	float retVal = cache[m_Shape.index(b, h, w, c)];

	delete[] cache;
	return retVal;
}

GRAVEngine::AI::Tensors::tensorShape GRAVEngine::AI::Tensors::tensor::shape()
{
	return m_Shape;
}

//float& GRAVEngine::AI::Tensors::tensor::operator[](size_t index)
//{	
//	GRAV_ASSERT(index < m_Shape.length());
//
//	return m_Data->
//}
//
//const float& GRAVEngine::AI::Tensors::tensor::operator[](size_t index) const
//{
//	// TODO: insert return statement here
//}
