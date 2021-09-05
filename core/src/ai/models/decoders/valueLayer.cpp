#include "gravpch.h"
#include "valueLayer.h"

GRAVEngine::AI::Models::Decoders::valueLayerImpl::valueLayerImpl(int64 numInput, int64 numOutput) : m_Layer(numInput, numOutput)
{
	// Register the module
	register_module("Value Layer", m_Layer);
}

torch::Tensor GRAVEngine::AI::Models::Decoders::valueLayerImpl::forward(torch::Tensor hidden)
{
	// Output the last value from the network
	return m_Layer->forward(hidden);
}
