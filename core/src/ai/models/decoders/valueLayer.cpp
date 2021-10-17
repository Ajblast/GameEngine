#include "gravpch.h"
#include "valueLayer.h"

GRAVEngine::AI::Models::Decoders::valueLayerImpl::valueLayerImpl(int64 numInput, int64 numOutput) : m_Layer(numInput, numOutput)
{
	torch::nn::init::kaiming_normal_(m_Layer->weight, 0, torch::kFanIn, torch::kLinear);
	//torch::nn::init::kaiming_normal_(m_Layer->weight);
	torch::nn::init::constant_(m_Layer->bias, 0);
	//torch::nn::init::uniform_(m_Layer->weight);
	//torch::nn::init::constant_(m_Layer->bias, 0.1);

	// Register the module
	register_module("Value Layer", m_Layer);
}

torch::Tensor GRAVEngine::AI::Models::Decoders::valueLayerImpl::forward(torch::Tensor hidden)
{
	// Output the last value from the network
	return m_Layer->forward(hidden);
}
