#pragma once

#include "common.h"

#include "rendering/textures/texture2D.h"
#include "rendering/cameras/camera.h"

#include "types/geometry.h"
#include "types/vecs/vec3.h"
#include "types/mats/mat4x4.h"

#include <glm/glm.hpp>

namespace GRAVEngine
{
	namespace Rendering
	{
		// TODO: Finish implementation of the 2D rendering

		// A renderer for 2D graphics
		class renderer3D
		{
		public:
			// Startup the 2D renderer
			static void startup();
			// Shutdown the 2D renderer
			static void shutdown();

			// Begin drawing a scene with a transform
			static void beginScene(const camera& camera);
			// End drawing the current scene
			static void endScene();
			// Flush the scene to the buffer
			static void flush();

			static void drawMesh(const vec3& position, const ref<mesh>& mesh, const RGBA& color);
			static void drawMesh(const mat4x4& transform, const ref<mesh>& mesh, const RGBA& color);
			static void drawMesh(const mat4x4& transform, const ref<mesh>& mesh, const ref<texture2D>& texture, float tilingFactor = 1.0f, const RGBA& tintColor = RGBA(1.0f));

			// Stats
			struct statistics
			{
				uint32 m_DrawCalls = 0;
				uint32 m_TriangleCount = 0;

				uint32 getTotalVertexCount() const { return m_TriangleCount * 3; }
				uint32 getTotalIndexCount() const { return m_TriangleCount * 3; }
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