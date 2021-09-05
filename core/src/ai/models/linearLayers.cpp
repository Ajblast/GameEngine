#include "gravpch.h"
#include "linearLayers.h"

// Free function to initialize the weights of the linear layers
void initWeights(torch::nn::Module& module)
{
	torch::NoGradGuard no_grad;
	if (auto* linear = module.as<torch::nn::Linear>())
	{
		linear->weight.normal_(0, 0.1);
		torch::nn::init::constant_(linear->bias, 0.1);
	}
}

GRAVEngine::AI::Models::linearLayersImpl::linearLayersImpl(int64 numInputs, int64 numLayers, int64 hiddenSize)
{
	// Add the first input layer of transforming the input to the hidden network size
	// This means that no matter what, there will always be at least a single layer
	m_Layers->push_back(torch::nn::Linear(numInputs, hiddenSize));
	m_Layers->push_back(torch::nn::ReLU()); // Use a Rectified linear unit activation function

	// Create all of the layers
	for (size_t i = 0; i < (size_t) numLayers - 1; i++)
	{
		// Add a new linear layer and activation function
		m_Layers->push_back(torch::nn::Linear(hiddenSize, hiddenSize));
		m_Layers->push_back(torch::nn::ReLU());
	}

	// Register the sequential network
	register_module("Linear Layers", m_Layers);

	// Initialize the weights for the sequential netowrk
	apply(initWeights);
}

torch::Tensor GRAVEngine::AI::Models::linearLayersImpl::forward(torch::Tensor inputs)
{
	// Forward the inputs through the sequential layer
	return m_Layers->forward(inputs);
}
