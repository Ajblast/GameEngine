#include "gravpch.h"
#include "orthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

void GRAVEngine::Rendering::orthographicCamera::setProjection(float left, float right, float bottom, float top)
{
	m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}