#pragma once

#include "common.h"
#include "distribution.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Distributions
		{
			// A normal distribution
			class GRAVAPI normal : public distribution
			{
			public:
				// Get a normal distribution from the location and scale
				normal(torch::Tensor location, torch::Tensor scale);
				virtual ~normal() = default;

				// Distribution mean
				virtual torch::Tensor mean() override;
				// Distribution variance
				virtual torch::Tensor variance() override;
				// Distribution standard deviation
				virtual torch::Tensor stddev() override;

				// Take a sample from the distribution
				virtual torch::Tensor sample(c10::IntArrayRef sampleShape = c10::IntArrayRef()) override;
				// What are the log probabilities of the values in the tensor
				virtual torch::Tensor logProb(torch::Tensor value) override;
				// What is the distributions entropy
				virtual torch::Tensor entropy() override;
			private:
				torch::Tensor m_Location;	// Location in the normal distribution
				torch::Tensor m_Scale;		// The scale/standard deviation of the normal distribution

			};
		}
	}
}