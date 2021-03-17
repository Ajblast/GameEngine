#include "gravpch.h"
#include "renderer.h"

// Create the scene data
GRAVEngine::Rendering::renderer::sceneData* GRAVEngine::Rendering::renderer::s_SceneData = new GRAVEngine::Rendering::renderer::sceneData();

void GRAVEngine::Rendering::renderer::startup()
{
	// Startup the abstraction for renderer commands
	rendererCommand::startup();

	// TODO: Allow picking which renderer is being used. Explicity, allowing 2D and 3D renderers
	// Startup a 2D renderer
	renderer2D::startup();
}

void GRAVEngine::Rendering::renderer::shutdown()
{
	// TODO: Allow picking which renderer is being used. Explicity, allowing 2D and 3D renderers
	// Shutdown a 2D renderer
	renderer2D::shutdown();
}

void GRAVEngine::Rendering::renderer::onWindowResize(uint32 width, uint32 height)
{
	// Tell the renderer to set the viewport
	rendererCommand::setViewPort(0, 0, width, height);
}

void GRAVEngine::Rendering::renderer::beginScene(orthographicCamera& camera)
{
	// TODO: Cameras
	s_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
}

void GRAVEngine::Rendering::renderer::endScene()
{
}

void GRAVEngine::Rendering::renderer::submit(const ref<shader>& shader, const ref<vertexArray>& vertexArray, const glm::mat4& transform)
{
	// Bind the shader
	shader->bind();

	// Set the view projection matrix
	shader->setMat4("u_ViewProjection", s_SceneData->viewProjectionMatrix);

	// Set the transform
	shader->setMat4("u_Transform", transform);

	// Bind the vertex array for rendering
	vertexArray->bind();
	rendererCommand::drawIndexed(vertexArray);
}
