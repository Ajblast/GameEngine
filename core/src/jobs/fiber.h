#pragma once

#include "common.h"
#include "jobTypes.h"

namespace GRAVEngine
{
	namespace Jobs
	{
		// Forward declare job manager for friendship
		class jobManager;

		// TODO: Add the potential for there to be fiber's with different stack sizes

		// A fiber is a piece of execution context with its own stack that is called explicitly
		class GRAVAPI fiber
		{
			friend jobManager;

		public:
			// Callback for fiber function
			//typedef std::function<void(fiber*)> fiberCallback;
			using fiberCallback = void(*)(fiber*);
		public:
			fiber();
			fiber(const fiber& other) = delete;				// Not allowed to copy construct
			fiber(fiber&& other) = delete;					// Not allowed to move construct
			fiber& operator=(const fiber& other) = delete;	// Not allowed to copy set
			fiber& operator=(fiber&& other) = delete;		// Not allowed to move set
			~fiber();


			void spawn(fiberCallback callback);
			// Initialize the current thread and make it a fiber
			void initializeFromCurrentThread();
			// Convert the current fiber to a thread
			void convertToThread();

			// Switch to a fiber from this current one
			void switchTo(fiber* fiber);

			// Is the current fiber valid
			inline bool isValid() const { return m_FiberHandle; }

			// Get the fiber's index
			const fiberIndex getIndex() const { return m_Index; }
		private:
			// Set the fiber's index
			void setIndex(fiberIndex index) { m_Index = index; }

		private:
			fiberHandle m_FiberHandle;
			fiberIndex m_Index;

			// Is this a fiber created from a thread
			bool m_IsThreadFiber = false;


			// TODO: Add fiber debug string for identification
		};
	}
}