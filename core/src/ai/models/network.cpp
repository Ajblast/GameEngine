#include "gravpch.h"
#include "network.h"

GRAVEngine::AI::Models::networkImpl::networkImpl(Training::networkSettings settings) : m_InputLayer(settings)
{
	// Create the linear layers
	m_LinearLayers = linearLayers(m_InputLayer->totalInputSize(), settings.m_LayerCount, settings.m_HiddenCount);

	// Register the input and linear layers
	register_module("Input", m_InputLayer);
	register_module("Linear Layers", m_LinearLayers);
}

torch::Tensor GRAVEngine::AI::Models::networkImpl::forward(std::vector<torch::Tensor> inputs)
{
	// Encode the inputs
	torch::Tensor encodedInputs = m_InputLayer->forward(inputs);

	// Take the encoded inputs and pass them through the linear layers
	torch::Tensor encodedOutputs = m_LinearLayers(encodedInputs);

	return encodedOutputs;
}