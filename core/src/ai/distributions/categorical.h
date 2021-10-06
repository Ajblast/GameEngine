#pragma once

#include "common.h"
#include "distribution.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Distributions
		{
			class GRAVAPI categorical : public distribution
			{
			public:
				categorical(torch::Tensor logits);
				virtual ~categorical() = default;


				// Distribution mean. Does nothing in categorical
				virtual torch::Tensor mean() override;
				// Distribution variance. Does nothing in categorical
				virtual torch::Tensor variance() override;
				// Distribution standard deviation. Does nothing in categorical
				virtual torch::Tensor stddev() override;

				// Take a sample from the distribution. TODO: Let categorical sample sample more than 1 item. It just ignores this for now because match is hard.
				virtual torch::Tensor sample(c10::IntArrayRef sampleShape = c10::IntArrayRef()) override;
				// What are the log probabilities of the values in the tensor
				virtual torch::Tensor logProb(torch::Tensor value) override;
				// What is the distributions entropy
				virtual torch::Tensor entropy() override;
			private:
				torch::Tensor m_Logits;			// Logistic probabilities
				torch::Tensor m_Probabilities;	// Probabilities
			};
		}
	}
}