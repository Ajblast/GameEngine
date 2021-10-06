#pragma once

#include "common.h"
#include "ai/actions/actionSpec.h"
#include "ai/models/actorcritic/actorOutput.h"
#include "ai/models/distributions/normalLayer.h"
#include "ai/models/distributions/multicategoricalLayer.h"
#include <torch/torch.h>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Models
		{
			namespace Decoders
			{
				// An action layer will take the hidden outputs from a network and generate the actions, log probabilities, and entropy
				class GRAVAPI actionLayerImpl : public torch::nn::Module
				{
				public:
					// Required default constructor
					actionLayerImpl() = default;
					// Create an action layer with the number of inputs and the actions spec output
					actionLayerImpl(int64 numInput, Actions::actionSpec actionSpec);
					// Default deconstructor
					virtual ~actionLayerImpl() = default;

					// Taking the output of a network's hidden layer, determine the actions, log probabilities, and entropy
					ActorCritic::actorOutput forward(torch::Tensor hidden);

					// Get the log prob of an action
					ActorCritic::agentLogProbs logProbs(torch::Tensor hidden, ActorCritic::agentAction action);
				private:
					normalLayer m_ContinuousLayer;			// The output layer for continous actions
					multicategoricalLayer m_DiscreteLayer;	// The output layer for discrete actions
					Actions::actionSpec m_ActionSpec;		// The action spec
				};
				TORCH_MODULE(actionLayer);
			}
		}
	}
}