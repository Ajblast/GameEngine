#include "gravpch.h"
#include "editorCamera.h"

#include "io/input.h"
#include "io/keyCodes.h"
#include "io/mouseCodes.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

GRAVEngine::Rendering::editorCamera::editorCamera(float fov, float aspectRatio, float nearClip, float farClip) :
	m_FOV(fov),
	camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip), glm::mat4(1.0f), aspectRatio, nearClip, farClip)
{
	recalculateViewMatrix();
}

void GRAVEngine::Rendering::editorCamera::OnUpdate(Time::timestep timestep)
{
	GRAV_PROFILE_FUNCTION();

	if (IO::Input::isKeyPressed(Keys::LeftAlt))
	{
		// Get the mouse delta
		const glm::vec2& mouse{ IO::Input::getMouseX(), IO::Input::getMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;

		// Check what type of operation needs to be done
		if (IO::Input::isMouseButtonPressed(Mouse::ButtonMiddle))
			mousePan(delta);
		else if (IO::Input::isMouseButtonPressed(Mouse::ButtonLeft))
			mouseRotate(delta);
		else if (IO::Input::isMouseButtonPressed(Mouse::ButtonRight))
			mouseZoom(delta.y);
	}

	// Update the view matrix
	recalculateViewMatrix();
}


void GRAVEngine::Rendering::editorCamera::recalculateProjectionMatrix()
{
	GRAV_PROFILE_FUNCTION();

	m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	
	recalculateViewProjectionMatrix();
}


