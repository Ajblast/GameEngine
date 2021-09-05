#include "gravpch.h"
#include "normalLayer.h"
#include "ai/distributions/normal.h"

GRAVEngine::AI::Models::Decoders::normalLayerImpl::normalLayerImpl(int64 numInput, int64 numContinuous) : m_Mu(numInput, numContinuous), m_Log(torch::zeros({ 1, numContinuous }, torch::requires_grad(true)))
{
	// Register the modules
	register_module("mu", m_Mu);
	register_parameter("Log", m_Log);
}

GRAVEngine::ref<GRAVEngine::AI::Distributions::distribution> GRAVEngine::AI::Models::Decoders::normalLayerImpl::forward(torch::Tensor inputs)
{
	// Get the mean
	torch::Tensor mu = m_Mu(inputs);

	// Get the standard deviation
	torch::Tensor std = m_Log.exp().expand_as(mu);

	// Create the normal distribution
	return createRef<Distributions::normal>(mu, std);
}
