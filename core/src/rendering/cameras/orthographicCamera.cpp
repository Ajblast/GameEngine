#include "gravpch.h"
#include "orthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

GRAVEngine::Rendering::orthographicCamera::orthographicCamera(float left, float right, float bottom, float top) : 
	camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f), glm::mat4(1.0f), 1, -1.0f, 1.0f)
{
	recalculateViewMatrix();
}

void GRAVEngine::Rendering::orthographicCamera::setProjection(float left, float right, float bottom, float top)
{
	GRAV_PROFILE_FUNCTION();

	m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}