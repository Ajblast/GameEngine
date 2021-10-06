#pragma once

#include "common.h"

#ifdef GRAV_PLATFORM_WINDOWS
#ifndef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif
#endif

namespace GRAVEngine
{
	namespace Jobs
	{
		typedef uint16 fiberIndex;
		typedef void* fiberHandle;

		class GRAVAPI fiber
		{
		public:
			// Callback for fiber function
			using fiberFunction = void(*)(fiber*);

		public:
			fiber();
			fiber(const fiber& other) = delete;
			//fiber(fiber&& other);

			fiber& operator=(const fiber& other) = delete;
			//fiber& operator=(fiber&& other);
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

			void setIndex(fiberIndex index) { m_Index = index; }
			const fiberIndex getIndex() const { return m_Index; }
		private:
			//fiber(fiberHandle fiber);

		private:
			fiberHandle m_FiberHandle;
			fiberFunction m_FunctionCallback;
			fiber* m_CallingFiber;

			// Is this a fiber created from a thread
			bool m_IsThreadFiber = false;

			fiberIndex m_Index;

			// TODO: Add fiber debug string for identification
		};
	}
}