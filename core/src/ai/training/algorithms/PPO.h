#pragma once

#include "common.h"
#include "ppoHyperparameters.h"
#include "ai/training/networkSettings.h"
#include "ai/training/ITrainingAlgorithm.h"
#include "ai/models/actorcritic/actorCritic.h"
#include "ai/training/updateBuffer.h"
#include "ai/inferenceDevice.h"

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			namespace Algorithms
			{
				// The PPO training algorithm
				class GRAVAPI PPO : public ITrainingAlgorithm
				{
				public:
					PPO(networkSettings settings, ref<ppoHyperparameters> hyperparameters);
					virtual ~PPO() = default;
					
					// Get the algorithm type
					virtual algorithmType getAlgorithmType() override;
					// Get the network settings
					virtual networkSettings getNetworkSettings() override;
					// Get the hyperparameters
					virtual hyperparameters& getHyperparameters() override;
					// Should an update be triggered
					virtual bool shouldUpdate() override;

					// Run through the algorithm's model
					virtual Models::ActorCritic::actorCriticOputput forward(std::vector<torch::Tensor> inputs) override;

					// Print the model
					virtual void print() const override;

					// Update the model with a trajectory
					virtual updateLoss update() override;

					// Create a model for the algorithm
					virtual torch::nn::AnyModule getModel() override { return torch::nn::AnyModule(m_Model); }
					// Get the optimizer
					virtual torch::optim::Optimizer& getOptimizer() override { return m_Optimizer; }

					// Save the model to filePath
					virtual void saveModel(const std::string& filePath) override;
					// Load a model from filePath
					virtual void loadModel(const std::string& filePath) override;

					virtual void addTrajectory(trajectory trajectory) override;

					virtual void sendToDevice(inferenceDevice device) override;
				private:
					std::vector<torch::Tensor> gae(trajectory trajectory);
				private:
					Models::ActorCritic::actorCritic m_Model;	// The model for training
					ref<ppoHyperparameters> m_Hyperparameters;		// Hyperparameters for the algorithm
					networkSettings m_Settings;						// Algorithm settings

					torch::optim::Adam m_Optimizer;

					updateBuffer m_UpdateBuffer;					// The update buffer that stores the values needed to update an algorithm
				};
			}
		}
	}
}