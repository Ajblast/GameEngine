#include "gravpch.h"
#include "linearLayers.h"

// Free function to initialize the weights of the linear layers
void initWeights(torch::nn::Module& module)
{
	torch::NoGradGuard no_grad;
	if (auto* linear = module.as<torch::nn::Linear>())
	{
		torch::nn::init::kaiming_normal_(linear->weight, 0.0, torch::kFanIn, torch::kLinear);
		//torch::nn::init::kaiming_normal_(linear->weight);
		torch::nn::init::constant_(linear->bias, 0);
		//torch::nn::init::uniform_(linear->weight, 0, 0.1);

		//linear->weight.normal_(0, 0.1);
		//torch::nn::init::constant_(linear->bias, 0.1);
	}
}

void printLinear(torch::nn::Module& module)
{
	torch::NoGradGuard no_grad;
	if (auto* linear = module.as<torch::nn::Linear>())
	{
		std::cout << "Weights: " << linear->weight << std::endl;
		std::cout << "Weight Gradient: " << linear->weight.grad() << std::endl;
		std::cout << "Bias: " << linear->bias << std::endl;
		std::cout << "Bias Gradient: " << linear->bias.grad() << std::endl;
	}
}

GRAVEngine::AI::Models::linearLayersImpl::linearLayersImpl(int64 numInputs, int64 numLayers, int64 hiddenSize)
{
	// Add the first input layer of transforming the input to the hidden network size
	// This means that no matter what, there will always be at least a single layer
	m_Layers->push_back(torch::nn::Linear(numInputs, hiddenSize));
	m_Layers->push_back(torch::nn::ReLU()); // Use a Rectified linear unit activation function
	//m_Layers->push_back(torch::nn::LeakyReLU()); // Use a Rectified linear unit activation function

	// Create all of the layers
	for (size_t i = 0; i < (size_t)numLayers - 1; i++)
	{
		// Add a new linear layer and activation function
		m_Layers->push_back(torch::nn::Linear(hiddenSize, hiddenSize));
		m_Layers->push_back(torch::nn::ReLU());
		//m_Layers->push_back(torch::nn::LeakyReLU());
	}

	// Register the sequential network
	register_module("Linear Layers", m_Layers);

	// Initialize the weights for the sequential netowrk
	apply(initWeights);
}

torch::Tensor GRAVEngine::AI::Models::linearLayersImpl::forward(torch::Tensor inputs)
{
	// Forward the inputs through the sequential layer
	torch::Tensor retVal = m_Layers->forward(inputs);

	if (retVal.isinf().any().item<bool>())
	{
		throw std::exception("Infinite Tensor");
	}
	else if (retVal.isnan().any().item<bool>())
	{
		apply(printLinear);

		throw std::exception("Nan Tensor");
	}

	return retVal;
}
