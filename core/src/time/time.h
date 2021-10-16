#pragma once

#include "stopwatch.h"
#include "timer.h"

#include <atomic>

namespace GRAVEngine
{
	class application;

	namespace Time
	{
		class time
		{
			friend application;
		public:
			inline static float getDeltaTime() { return s_DeltaTime.load(std::memory_order_acquire); }
		private:
			inline static void setDeltaTime(float value) { s_DeltaTime.store(value, std::memory_order_release); }

			inline static float getOldFrameTime() { return s_OldTime.load(std::memory_order_acquire); }
			inline static void setOldFrameTime(float value) { s_OldTime.store(value, std::memory_order_release); }
		private:
			static std::atomic<float> s_DeltaTime;
			static std::atomic<float> s_OldTime;
		};
	}
}