#pragma once

#include "common.h"
#include "keyCodes.h"
#include "mouseCodes.h"
#include "types/vecs/vec2.h"
#include <unordered_map>
#include <atomic>

namespace GRAVEngine
{
	namespace IO
	{
		// The manager that runs on the main thread to poll events
		class GRAVAPI inputManager
		{
		public:
			// Startup the manager
			void startup();
			// Shutdown the manager
			void shutdown();
			// Do a single poll event
			void pollEvents();

			// Is the key pressed
			bool const isKeyPressed(const Keys::keyCode key) const;;
			// Is the mouse button pressed
			const bool isMouseButtonPressed(Mouse::mouseCode button) const;
			// Get the current mouse position
			const vec2 getMousePosition() const;
			// Get the mouse X
			const float getMouseX() const;
			// Get the mouse Y
			const float getMouseY() const;

			// Is the input manager valid?
			bool const isValid() const { return m_IsValid.load(std::memory_order_acquire); };
			// Get the manager instance
			static inputManager* getInstance() { return s_Instance; };
		private:
			static inputManager* s_Instance;											// Static instance

			std::atomic_bool m_IsValid;
			std::unordered_map<Keys::keyCode, std::atomic_bool> m_KeyPressedMap;		// Map from keycode to pressed
			std::unordered_map<Mouse::mouseCode, std::atomic_bool> m_MousePressedMap;	// Map from mouse button to pressed
			std::atomic<vec2> m_MousePosition;											// The mouse position
		};
	}
}