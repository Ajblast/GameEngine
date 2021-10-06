#pragma once

#include "common.h"
#include "ai/models/model.h"
#include "algorithmType.h"
#include "trajectory.h"
#include "networkSettings.h"
#include "hyperparameters.h"
#include "ai/inferenceDevice.h"
#include <sstream>

namespace GRAVEngine
{
	namespace AI
	{
		namespace Training
		{
			// Interface for a training algorithm
			class GRAVAPI ITrainingAlgorithm
			{
			public:
				ITrainingAlgorithm() = default;
				virtual ~ITrainingAlgorithm() = default;

			public:
				// Get the model
				//virtual Models::model& model() = 0;
				// Get the algorithm type
				virtual algorithmType getAlgorithmType() = 0;
				// Get the network settings
				virtual networkSettings getNetworkSettings() = 0;
				// Get the hyperparameters
				virtual hyperparameters& getHyperparameters() = 0;
				// Should the algorithm be updated
				virtual bool shouldUpdate() = 0;
				// Send the algorithm to a inference device
				virtual void sendToDevice(inferenceDevice device) = 0;
				//// Is the algorithm initialized
				//virtual bool initialized() = 0;
			
			public:
				// Run through the algorithm's model
				virtual Models::ActorCritic::actorCriticOputput forward(std::vector<torch::Tensor> inputs) = 0;

				// Print the model
				virtual void print() const = 0;

				// Update the model with a trajectory
				virtual void update() = 0;
				//// Create a model for the algorithm
				//virtual ref<Models::model> createModel() = 0;
				// Save the model to filePath
				virtual void saveModel(const std::string& filePath) = 0;
				// Load a model from filePath
				virtual void loadModel(const std::string& filePath) = 0;
				// Add a trajectory to the algorithm
				virtual void addTrajectory(trajectory trajectory) = 0;
				//// Initialize the training algorithm
				//virtual void initialize(networkSettings settings, ref<hyperparameters> parameters) = 0;
			};
		}
	}
}