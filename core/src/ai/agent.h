#pragma once

#include "common.h"
#include "IRewardable.h"
#include "IEpisodic.h"
#include "actions/IActionReciever.h"
#include "doneReason.h"
#include "actions/actuatorList.h"
#include "sensors/ISensor.h"
#include "sensors/vectorSensor.h"
#include "programParams.h"
#include "agentParams.h"
#include "agentInfo.h"
#include "agentPrograms/IAgentProgram.h"
#include "ai/models/model.h"
#include "ai/environmentManager.h"



namespace GRAVEngine
{
	namespace AI
	{
		// Agent used for
		class GRAVAPI agent : public IRewardable, IEpisodic
		{
		public:
			// Default agent that can only use collectObservations
			agent(programParams programParams, size_t maxStep = 0);
			// Agent with premade sensors. Will initialize
			agent(std::vector<ref<Sensors::ISensor>>& sensors, std::vector<ref<Actions::IActuator>>& actuators, programParams programParams, size_t maxStep = 0);
			//agent(agent&& other) noexcept;
			//agent& operator=(agent&& other) noexcept;

			virtual ~agent();

			// Safe initiailze the agent and can be called multiple times
			void safeInitialization();
			// Initialze the agent. Meant for custom initialization. Sensors and actuators must have already been passed within the constructor.
			// Newly added sensors and actuators will not be allocated.
			virtual void initialize();
			// Deinitialize the agent. Able to initialize again
			void deinitialize();

			inline void requestAction() { m_ShouldRequestAction = true; }
			inline void requestDecision() { m_ShouldRequestDecision = true; requestAction(); }
			inline float currentReward() const { return m_Info.m_Reward; }
		private:
			// Update the sensors
			void updateSensors();
			// Reset the sensors
			void resetSensors();
			// Collect observations intot he sensor
			//virtual void collectObservations(Sensors::vectorSensor& sensor);

			// Tell the agent to send the infomation to the brain if a decision needs to be made
			void sendInfo();
			// The next step
			void incrementStep();
			// Call the next step
			void step();
			// Decide the action
			void decideAction();

			// Mark the agent as done
			void markDone(doneReason reason);
			// Set the neural network model
			//void setModel(Models::model model);

		public:
			// Add a list of sensors to the agent. Does nothing after initialization
			void initializeSensors(std::vector<ref<Sensors::ISensor>>& sensors);
			// Add a list of actuators to the agent. Does nothing after initialization
			void initializeActuators(std::vector<ref<Actions::IActuator>>& actuators);

		private:
			// Send info to the brain
			void sendInfoToBrain();

			// Sort the sensors
			void sortSensors();
			// Validate that the sensors are unique by name
			void validateSensors();

		public:
			// Set the reward
			virtual void setReward(float reward) override;
			// Add a reward
			virtual void addReward(float increment) override;
			// What is the cumulative reward
			float cumulativeReward() override;
			// What is the maximum amount of steps
			const float maxStep() const;

			// Called when the episode begins
			virtual void onEpisodeBegin() override;
			// Called when the episode ends
			virtual void onEpisodeEnd() override;
			// The total number of completed episodes
			virtual uint32 completedEpisdes() override;
			// End the episode
			virtual void endEpisode() override;
			// Interupt the episode
			virtual void interuptEpisode() override;
			// End the episode and reset the agent
			virtual void endEpisodeAndReset(doneReason reason) override;

			// Called when an action is recieved
			//virtual void onActionRecieved(Actions::actionBuffer buffers) override;

			// Called to collect some internal observations without the need of a sensor
		private:
			// Reset the agent and update internal data structures. Starts another episode.
			void reset();
			// Reset the internal data structures 
			void resetData();

		private:
			scope<IAgentProgram> m_Brain;				// Brain of the agent
			agentInfo m_Info;							// Informaiton sent to the brain
			Actions::actuatorList m_Actuators;			// Agent Actuators
			std::vector<ref<Sensors::ISensor>> m_Sensors;	// Agent Sensors

			bool m_ShouldRequestAction;			// Should an action be requested
			bool m_ShouldRequestDecision;		// Should a decision be requested
			bool m_Initialized;					// Is the agent initialized
			agentParams m_AgentParams;			// Parameter's for the agent
			programParams m_BehaviorParams;		// Parameter's for the agent's behavior

			GRAVEngine::PubSub::eventHandle m_StepHandle;
			GRAVEngine::PubSub::eventHandle m_SendInfoHandle;
			GRAVEngine::PubSub::eventHandle m_DecideActionHandle;
			GRAVEngine::PubSub::eventHandle m_ActHandle;
			GRAVEngine::PubSub::eventHandle m_ResetHandle;

		};
	}
}