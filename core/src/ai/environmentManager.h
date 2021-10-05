#pragma once

#include "common.h"
#include "pubsub/event.h"
#include "locks/spinLock.h"
#include "locks/scopedLock.h"
#include "ai/training/trainerController.h"

#include "jobs/jobs.h"

namespace GRAVEngine
{
	namespace AI
	{
		// Trainer used to train agents in an environment
		class environmentManager
		{
		public:
			environmentManager(const environmentManager&) = delete;				// Delete copy constructor
			environmentManager& operator= (const environmentManager&) = delete;	// Delete set constructor

			// Initialize the trainer
			void initialize(scope<Training::algorithmFactory> algorithmFactory);
			// Deinitialize the trainer
			void deinitialize();

			inline const bool initialized() const { return m_Initialized; }
			inline static environmentManager& instance() { return s_Instance; }
			inline const uint32 episodeCount() const { return m_EpisodeCount; }
			inline const uint32 stepCount() const { return m_StepCount; }
			inline const uint32 totalStepCount() const { return m_TotalStepCount; }

			Training::trainerController& controller();
		public:
			PubSub::event<> m_DecideActionEvent;		// Agents are to decide their decision
			PubSub::event<> m_DestroyActionEvent;		// The trainer is being destroyed
			PubSub::event<> m_AgentIncrementStepEvent;	// New step is about to start

			PubSub::event<> m_AgentSendInfoEvent;		// Can send their state to their brains
			PubSub::event<> m_AgentActEvent;			// Agent can act if decision requested
			PubSub::event<> m_AgentResetEvent;			// The trainer is forcing a reset

			PubSub::event<> m_OnEnvironmentReset;		// The environment for the training has been reset

			void step();
			void reset();
		private:
			explicit environmentManager();
			~environmentManager();

		private:
			Locks::spinLock m_TrainerLock;	// Lock to make sure that only a single thread can talk to the trainer at a time

			bool m_Initialized;
			static environmentManager& s_Instance;	// TODO: Fix how the instance should be done.

			uint32 m_EpisodeCount;		// Number of episodes done by this trainer
			uint32 m_StepCount;			// The current episode step count
			uint32 m_TotalStepCount;	// The total step count of the trainer

			scope<Training::trainerController> m_TrainerController;
		};
	}
}