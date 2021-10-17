#pragma once

#include "common.h"
#include "rendering/renderer/rendererAPI.h"
#include "rendering/renderer/rendererCommand.h"
#include "rendering/renderer/renderer2D.h"
#include "rendering/renderer/renderer3D.h"
#include "rendering/shaders/shader.h"
#include "rendering/cameras/camera.h"
#include <glm/mat4x4.hpp>

namespace GRAVEngine
{
	namespace Rendering
	{
		// TODO: Multithread renderer

		// Genertic interface for a renderer. A Renderer uses a rendering API
		// Examples include 2D renderers, 3D renderers, wireframe renderers
		class GRAVAPI renderer
		{
		public:
			// Statup the renderer
			static void startup();
			// Shutdown the renderer
			static void shutdown();

			// Callback for when the window resizes
			static void onWindowResize(uint32 width, uint32 height);

			// Begin drawing a scene with an orthographic camera
			static void beginScene(camera& camera);
			// End drawing the scene
			static void endScene();

			//// Submit a vertex array for rendering with a specific shader
			//// TODO: Implement an asynchronous way to submit data. i.e., allow storage of vertex information such that it can all be done in one step without blocking
			//static void submit(const ref<shader>& shader, const ref<vertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

			// Get the current rendering API
			static rendererAPI::API getAPI() { return rendererAPI::getAPI(); }

		private:
			struct sceneData
			{
				glm::mat4 viewProjectionMatrix;
			};

			static sceneData* s_SceneData;
		};
	}
}