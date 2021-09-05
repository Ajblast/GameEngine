#include "gravpch.h"
#include "inputLayer.h"
#include "ai/models/encoders/encoders.h"

GRAVEngine::AI::Models::inputLayerImpl::inputLayerImpl(Training::networkSettings settings)
{
	std::vector<torch::nn::Module> inputEncoders;
	int32 totalInputSize = 0;

	// For each observation spec, create an encoder for it. TODO: Allow different encoders
	for (auto it = settings.m_ObservationSpecs.begin(); it != settings.m_ObservationSpecs.end(); it++)
	{
		// Add a vector encoder. TODO: Add functionality to choose a different encoder depending on the observatin spec
		m_Modules->push_back(Encoders::vectorEncoder());

		// Add the size of the shape to the total input size
		totalInputSize += (*it).m_Shape.length();

	}

	// Set the total input size
	m_TotalInputSize = torch::full({ 1 }, totalInputSize, torch::kInt);

	// Register the list of input encoders
	register_module("InputEncoders", m_Modules);
	// Register the input size
	register_buffer("TotalInputSize", m_TotalInputSize);
}

torch::Tensor GRAVEngine::AI::Models::inputLayerImpl::forward(std::vector<torch::Tensor> inputs)
{
	GRAV_ASSERT(inputs.size() == m_Modules->size()); // Make sure the number of inputs matches the amount of encoders

	std::vector<torch::Tensor> encodedInputs; // The encoded inputs

	//for (size_t i = 0; i < inputs.size(); i++)
	//{
	//	std::cout << "Inputs Input " << i << ":\n" << inputs[i] << std::endl;
	//}

	// Iterate over each encoder and pass its corresponding input
	size_t inputIndex = 0;
	for (auto it = m_Modules->begin(); it != m_Modules->end(); it++)
	{
		// Add the encoded input to the list of inputs
		encodedInputs.push_back(
			(*it)->as<Encoders::encoder>()->forward(inputs[inputIndex++])
		);
	}

	//for (size_t i = 0; i < encodedInputs.size(); i++)
	//{
	//	std::cout << "Encouded Input " << i << ":\n" << encodedInputs[i] << std::endl;
	//}

	// Return a tensor from all of the inputs concatenated together into one big tensor
	return torch::cat(encodedInputs, 1);
}

//size_t GRAVEngine::AI::Models::inputLayerImpl::totalInputSize() const { return m_TotalInputSize.item<int64>(); }
