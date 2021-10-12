#include "gravpch.h"
#include "inputManager.h"
#include "application.h"
#include <GLFW/glfw3.h>

GRAVEngine::IO::inputManager* GRAVEngine::IO::inputManager::s_Instance = nullptr;

void GRAVEngine::IO::inputManager::startup()
{
	GRAV_PROFILE_FUNCTION();

	GRAV_ASSERT(s_Instance == nullptr);
	GRAV_LOG_LINE_INFO("%s: Begin Startup", GRAV_CLEAN_FUNC_SIG);

	s_Instance = this;

	// Initialize Keycode map
	for (auto it = Keys::g_KeyCodes.begin(); it != Keys::g_KeyCodes.end(); it++)
		m_KeyPressedMap.emplace(*it, false);

	// Initialize Mousecode map
	for (auto it = Mouse::g_MouseCodes.begin(); it != Mouse::g_MouseCodes.end(); it++)
		m_MousePressedMap.emplace(*it, false);

	m_MousePosition.store({ 0, 0 }, std::memory_order_release);
	m_IsValid.store(true, std::memory_order_release);

	GRAV_LOG_LINE_INFO("%s: End Startup", GRAV_CLEAN_FUNC_SIG);
}
void GRAVEngine::IO::inputManager::shutdown()
{
	GRAV_PROFILE_FUNCTION();

	GRAV_ASSERT(s_Instance);
	GRAV_LOG_LINE_INFO("%s: Begin Shutdown", GRAV_CLEAN_FUNC_SIG);

	// Null out intput
	for (auto it = m_KeyPressedMap.begin(); it != m_KeyPressedMap.end(); it++)
		it->second.store(false, std::memory_order_release);
	for (auto it = m_MousePressedMap.begin(); it != m_MousePressedMap.end(); it++)
		it->second.store(false, std::memory_order_release);
	m_MousePosition.store({ 0, 0 }, std::memory_order_release);

	m_IsValid.store(false, std::memory_order_release);

	s_Instance = nullptr;

	GRAV_LOG_LINE_INFO("%s: End Shutdown", GRAV_CLEAN_FUNC_SIG);
}

void GRAVEngine::IO::inputManager::pollEvents()
{
	GRAV_PROFILE_FUNCTION();
	GRAV_ASSERT(isValid());

	auto* window = static_cast<GLFWwindow*>(application::getInstance().getWindow().getNativeWindow());

	// Keycodes
	for (auto it = m_KeyPressedMap.begin(); it != m_KeyPressedMap.end(); it++)
	{
		auto state = glfwGetKey(window, static_cast<int32>(it->first));

		// Store the state
		it->second.store(state == GLFW_PRESS || state == GLFW_REPEAT, std::memory_order_release);
	}
	// Mouse codes
	for (auto it = m_MousePressedMap.begin(); it != m_MousePressedMap.end(); it++)
	{
		auto state = glfwGetMouseButton(window, static_cast<int32>(it->first));
		bool result = state == GLFW_PRESS;
		if (result)
			GRAV_LOG_LINE_INFO("************************************************************************************* Pressed!");

		// Store the state
		it->second.store(state == GLFW_PRESS, std::memory_order_release);
	}

	// Mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	m_MousePosition.store({ (float)xpos, (float)ypos }, std::memory_order_release);
}

bool const GRAVEngine::IO::inputManager::isKeyPressed(const Keys::keyCode key) const
{
	return m_KeyPressedMap.at(key).load(std::memory_order_acquire); 
}
const bool GRAVEngine::IO::inputManager::isMouseButtonPressed(Mouse::mouseCode button) const
{
	return m_MousePressedMap.at(button).load(std::memory_order_acquire);;
}
const GRAVEngine::vec2 GRAVEngine::IO::inputManager::getMousePosition() const
{
	return m_MousePosition.load(std::memory_order_acquire);
}
const float GRAVEngine::IO::inputManager::getMouseX() const
{
	return getMousePosition().x;
}
const float GRAVEngine::IO::inputManager::getMouseY() const
{
	return getMousePosition().y;
}
