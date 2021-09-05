#include "gravpch.h"
#include "categorical.h"

GRAVEngine::AI::Distributions::categorical::categorical(torch::Tensor logits) : distribution(logits.sizes()), m_Logits(logits - logits.logsumexp(-1, true))
{
	// Spcial math for probabilities means can't initialize like normal
	m_Probabilities = torch::softmax(m_Logits, -1);
}

torch::Tensor GRAVEngine::AI::Distributions::categorical::mean()
{
	return torch::Tensor();
}
torch::Tensor GRAVEngine::AI::Distributions::categorical::variance()
{
	return torch::Tensor();
}

torch::Tensor GRAVEngine::AI::Distributions::categorical::stddev()
{
	return torch::Tensor();
}

torch::Tensor GRAVEngine::AI::Distributions::categorical::sample(c10::IntArrayRef sampleShape)
{
	return torch::multinomial(m_Probabilities, 1);
}

torch::Tensor GRAVEngine::AI::Distributions::categorical::logProb(torch::Tensor value)
{
	return torch::log(m_Probabilities);
}

torch::Tensor GRAVEngine::AI::Distributions::categorical::entropy()
{
	return -torch::sum(torch::log(m_Probabilities) * m_Probabilities, -1).unsqueeze(-1);
}
