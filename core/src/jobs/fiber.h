#pragma once

#include "common.h"

#ifdef _WIN32
#ifndef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#endif

namespace GRAVEngine
{
	namespace Jobs
	{
		typedef uint16 fiberIndex;
		typedef void* fiberHandle;

		class fiber
		{
		public:
			// Callback for fiber function
			using fiberFunction = void(*)(fiber*);

		public:
			fiber();
			fiber(const fiber& other) = delete;
			fiber(fiber&& other);

			fiber& operator=(const fiber& other) = delete;
			fiber& operator=(fiber&& other);
			~fiber();

			// Convert the current thread to a fiber
			void initializeFromCurrentThread();
			void convertToThread();

			// Fiber switching
			void switchTo(fiber* fiber);
			void switchToCallingFiber();

			void setCallback(fiberFunction callback);

			inline fiberFunction getCallback() const { return m_FunctionCallback; };
			inline bool isValid() const { return m_FiberHandle && m_FunctionCallback; }
		private:
			//fiber(fiberHandle fiber);

		private:
			fiberHandle m_FiberHandle;
			fiberFunction m_FunctionCallback;
			fiber* m_CallingFiber;

			// Is this a fiber created from a thread
			bool m_IsThreadFiber = false;

			// TODO: Add fiber debug string for identification
		};
	}
}