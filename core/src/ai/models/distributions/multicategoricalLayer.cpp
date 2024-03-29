#include "gravpch.h"
#include "multicategoricalLayer.h"
#include "ai/distributions/categorical.h"

GRAVEngine::AI::Models::Decoders::multicategoricalLayerImpl::multicategoricalLayerImpl(int64 numInput, std::vector<int64> branchSizes)
{
	GRAV_ASSERT(numInput > 0);
	GRAV_ASSERT(branchSizes.size() > 0);

	// Create a linear layer for each branch
	for (auto it = branchSizes.begin(); it != branchSizes.end(); it++)
	{
		torch::nn::Linear layer = torch::nn::Linear(numInput, *it);
		torch::nn::init::kaiming_normal_(layer->weight, 0, torch::kFanIn, torch::kLinear);
		//torch::nn::init::kaiming_normal_(layer->weight);
		//layer->weight = layer->weight * 0.1;
		torch::nn::init::constant_(layer->bias, 0);

		//torch::nn::init::uniform_(layer->weight);
		//torch::nn::init::constant_(layer->bias, 0.1);
		m_Branches->push_back(layer);
	}

	// Register the module list
	register_module("Branches", m_Branches);
}

std::vector<GRAVEngine::ref<GRAVEngine::AI::Distributions::distribution>> GRAVEngine::AI::Models::Decoders::multicategoricalLayerImpl::forward(torch::Tensor inputs)
{
	std::vector<ref<Distributions::distribution>> distributions;

	// Go through each branch
	for (auto it = m_Branches->begin(); it != m_Branches->end(); it++)
	{
		// Get the logits
		torch::Tensor logits = (*it)->as<torch::nn::Linear>()->forward(inputs);

		// Add the distribution
		distributions.push_back(createRef<Distributions::categorical>(logits));
	}

	return distributions;
}
