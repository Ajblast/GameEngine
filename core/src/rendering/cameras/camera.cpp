#include "gravpch.h"
#include "camera.h"
#include "io/input/input.h"
#include "io/input/keyCodes.h"
#include "io/input/mouseCodes.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

GRAVEngine::Rendering::camera::camera(const glm::mat4& projection, const glm::mat4& view, float aspectRatio, float nearClip, float farClip)
	: m_ProjectionMatrix(projection), m_ViewMatrix(view), m_ViewProjectionMatrix(projection* view), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
{}

void GRAVEngine::Rendering::camera::recalculateViewMatrix()
{
	GRAV_PROFILE_FUNCTION();

	m_Position = calculatePosition();

	glm::quat orientation = getOrientation();
	m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
	m_ViewMatrix = glm::inverse(m_ViewMatrix);

	recalculateViewProjectionMatrix();
}
void GRAVEngine::Rendering::camera::recalculateProjectionMatrix()
{

}
void GRAVEngine::Rendering::camera::recalculateViewProjectionMatrix()
{
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

glm::vec3 GRAVEngine::Rendering::camera::up() const
{
	return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}
glm::vec3 GRAVEngine::Rendering::camera::right() const
{
	return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}
glm::vec3 GRAVEngine::Rendering::camera::forward() const
{
	return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}
glm::quat GRAVEngine::Rendering::camera::getOrientation() const
{
	return glm::quat(glm::vec3(-getPitch(), -getYaw(), -getRoll()));
}



void GRAVEngine::Rendering::camera::onEvent(Events::event& event)
{
	Events::eventDispatcher dispatcher(event);
	dispatcher.dispatch<Events::mouseScrolledEvent>(GRAV_BIND_EVENT_FN(camera::onMouseScroll));
}
bool GRAVEngine::Rendering::camera::onMouseScroll(Events::mouseScrolledEvent& event)
{
	GRAV_PROFILE_FUNCTION();

	float delta = event.getYOffset() * 0.1f;
	mouseZoom(delta);
	recalculateViewMatrix();
	return false;
}
void GRAVEngine::Rendering::camera::OnUpdate(Time::timestep timestep)
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



void GRAVEngine::Rendering::camera::mousePan(const glm::vec2& delta)
{
	GRAV_PROFILE_FUNCTION();

	auto [xSpeed, ySpeed] = panSpeed();
	m_FocalPoint += -right() * delta.x * xSpeed * m_Distance;
	m_FocalPoint += up() * delta.y * ySpeed * m_Distance;
}
void GRAVEngine::Rendering::camera::mouseRotate(const glm::vec2& delta)
{
	GRAV_PROFILE_FUNCTION();

	float yawSign = up().y < 0 ? -1.0f : 1.0f;
	m_Rotation.y += yawSign * delta.x * rotationSpeed();
	m_Rotation.x += delta.y * rotationSpeed();
}
void GRAVEngine::Rendering::camera::mouseZoom(float delta)
{
	GRAV_PROFILE_FUNCTION();

	m_Distance -= delta * zoomSpeed();
	if (m_Distance < 1.0f)
	{
		m_FocalPoint += forward();
		m_Distance = 1.0f;
	}
}

glm::vec3 GRAVEngine::Rendering::camera::calculatePosition() const
{
	return m_FocalPoint - forward() * m_Distance;
}

std::pair<float, float> GRAVEngine::Rendering::camera::panSpeed() const
{
	float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
	float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

	float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
	float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	return { xFactor, yFactor };
}
float GRAVEngine::Rendering::camera::rotationSpeed() const
{
	return 0.8f;
}
float GRAVEngine::Rendering::camera::zoomSpeed() const
{
	float distance = m_Distance * 0.2f;
	distance = std::max(distance, 0.0f);
	float speed = distance * distance;
	speed = std::min(speed, 100.0f); // max speed = 100
	return speed;
}



