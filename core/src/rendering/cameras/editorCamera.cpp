#include "gravpch.h"
#include "editorCamera.h"

#include "io/input.h"
#include "io/keyCodes.h"
#include "io/mouseCodes.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

GRAVEngine::Rendering::editorCamera::editorCamera(float fov, float aspectRatio, float nearClip, float farClip) :
	m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), 
	camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip), glm::mat4(1.0f))
{
	updateView();
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
		if (IO::Input::isKeyPressed(Mouse::ButtonMiddle))
			mousePan(delta);
		else if (IO::Input::isKeyPressed(Mouse::ButtonLeft))
			mouseRotate(delta);
		else if (IO::Input::isKeyPressed(Mouse::ButtonRight))
			mouseZoom(delta.y);
	}

	// Update the view matrix
	updateView();
}
void GRAVEngine::Rendering::editorCamera::OnEvent(Events::event& event)
{
	Events::eventDispatcher dispatcher(event);
	dispatcher.dispatch<Events::mouseScrolledEvent>(GRAV_BIND_EVENT_FN(editorCamera::onMouseScroll));
}

glm::vec3 GRAVEngine::Rendering::editorCamera::up() const
{
	return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}
glm::vec3 GRAVEngine::Rendering::editorCamera::right() const
{
	return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}
glm::vec3 GRAVEngine::Rendering::editorCamera::forward() const
{
	return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::quat GRAVEngine::Rendering::editorCamera::getOrientation() const
{
	return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
}

void GRAVEngine::Rendering::editorCamera::updateProjection()
{
	GRAV_PROFILE_FUNCTION();

	m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}
void GRAVEngine::Rendering::editorCamera::updateView()
{
	GRAV_PROFILE_FUNCTION();

	// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
	m_Position = calculatePosition();

	glm::quat orientation = getOrientation();
	m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
	m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

bool GRAVEngine::Rendering::editorCamera::onMouseScroll(Events::mouseScrolledEvent& event)
{
	GRAV_PROFILE_FUNCTION();

	float delta = event.getYOffset() * 0.1f;
	mouseZoom(delta);
	updateView();
	return false;
}

void GRAVEngine::Rendering::editorCamera::mousePan(const glm::vec2& delta)
{
	GRAV_PROFILE_FUNCTION();

	auto [xSpeed, ySpeed] = panSpeed();
	m_FocalPoint += -right() * delta.x * xSpeed * m_Distance;
	m_FocalPoint += up() * delta.y * ySpeed * m_Distance;
}
void GRAVEngine::Rendering::editorCamera::mouseRotate(const glm::vec2& delta)
{
	GRAV_PROFILE_FUNCTION();

	float yawSign = up().y < 0 ? -1.0f : 1.0f;
	m_Yaw += yawSign * delta.x * rotationSpeed();
	m_Pitch += delta.y * rotationSpeed();
}
void GRAVEngine::Rendering::editorCamera::mouseZoom(float delta)
{
	GRAV_PROFILE_FUNCTION();

	m_Distance -= delta * zoomSpeed();
	if (m_Distance < 1.0f)
	{
		m_FocalPoint += forward();
		m_Distance = 1.0f;
	}
}

glm::vec3 GRAVEngine::Rendering::editorCamera::calculatePosition() const
{
	return m_FocalPoint - forward() * m_Distance;

}

std::pair<float, float> GRAVEngine::Rendering::editorCamera::panSpeed() const
{
	float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
	float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

	float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
	float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	return { xFactor, yFactor };
}
float GRAVEngine::Rendering::editorCamera::rotationSpeed() const
{
	return 0.8f;
}
float GRAVEngine::Rendering::editorCamera::zoomSpeed() const
{
	float distance = m_Distance * 0.2f;
	distance = std::max(distance, 0.0f);
	float speed = distance * distance;
	speed = std::min(speed, 100.0f); // max speed = 100
	return speed;
}
