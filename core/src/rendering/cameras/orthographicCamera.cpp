#include "gravpch.h"
#include "orthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

GRAVEngine::Rendering::orthographicCamera::orthographicCamera(float left, float right, float bottom, float top, float nearClip, float farClip) :
	camera(glm::ortho(left, right, bottom, top, nearClip, farClip), glm::mat4(1.0f), 1, nearClip, farClip)
{
	recalculateViewMatrix();
}

void GRAVEngine::Rendering::orthographicCamera::setProjection(float left, float right, float bottom, float top)
{
	GRAV_PROFILE_FUNCTION();

	// Set the projection with the saved near and far clip
	setProjection(left, right, bottom, top, m_NearClip, m_FarClip);

	recalculateViewProjectionMatrix();
}

void GRAVEngine::Rendering::orthographicCamera::setProjection(float left, float right, float bottom, float top, float nearClip, float farClip)
{
	GRAV_PROFILE_FUNCTION();

	// Claculate the projection matrix
	m_ProjectionMatrix = glm::ortho(left, right, bottom, top, nearClip, farClip);

	recalculateProjectionMatrix();
}
