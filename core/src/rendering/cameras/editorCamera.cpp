#include "gravpch.h"
#include "editorCamera.h"


#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

GRAVEngine::Rendering::editorCamera::editorCamera(float fov, float aspectRatio, float nearClip, float farClip) :
	m_FOV(fov),
	camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip), glm::mat4(1.0f), aspectRatio, nearClip, farClip)
{
	recalculateViewMatrix();
}

void GRAVEngine::Rendering::editorCamera::recalculateProjectionMatrix()
{
	GRAV_PROFILE_FUNCTION();

	m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	
	recalculateViewProjectionMatrix();
}


