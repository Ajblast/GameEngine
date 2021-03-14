#pragma once

#include "common.h"

#include "rendering/cameras/orthographicCamera.h"

#include "rendering/textures/texture2D.h"

#include "rendering/cameras/camera.h"
#include "rendering/cameras/editorCamera.h"

#include <glm/glm.hpp>

namespace GRAVEngine
{
	namespace Rendering
	{
		// TODO: Finish implementation of the 2D rendering

		// A renderer for 2D graphics
		class renderer2D
		{
		public:
			// Startup the 2D renderer
			static void startup();
			// Shutdown the 2D renderer
			static void shutdown();

			// Begin drawing a scene with a transform
			static void beginScene(const camera& camera, const glm::mat4& transform);
			// Being drawing a scene with the editor camera
			static void beginScene(const editorCamera& camera);
			static void beginScene(const orthographicCamera& camera); // TODO: Remove
			// End drawing the current scene
			static void endScene();
			// Flush the scene to the buffer
			static void flush();


			// Primitive drawing
			static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
			static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
			static void drawQuad(const glm::vec2& position, const glm::vec2& size, texture2D*& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
			static void drawQuad(const glm::vec3& position, const glm::vec2& size, texture2D*& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

			static void drawQuad(const glm::mat4& transform, const glm::vec4& color);
			static void drawQuad(const glm::mat4& transform, texture2D*& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

			static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
			static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
			static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, texture2D*& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
			static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, texture2D*& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

			// TODO: Drawing primitives
			/*
			* Line
			* Cross
			* Sphere
			* Circle
			* Axes
			* Triangle
			* AABB
			* OBB
			* String
			*/

			// Stats
			struct statistics
			{
				uint32 m_DrawCalls = 0;
				uint32 m_QuadCount = 0;

				uint32 getTotalVertexCount() const { return m_QuadCount * 4; }
				uint32 getTotalIndexCount() const { return m_QuadCount * 6; }
			};
			static void resetStats();
			static statistics getStats();

		private:
			// Start a new batch rendering
			static void startBatch();
			// Start the next batch of rendering
			static void nextBatch();
		};
	}
}