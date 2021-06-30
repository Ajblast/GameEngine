#pragma once

#include "common.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GRAVEngine
{
	namespace Rendering
	{
		class orthographicCamera : public camera
		{
		public:
			orthographicCamera(float left, float right, float bottom, float top) : camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f), glm::mat4(1.0f), 1, -1.0f, 1.0f) {}

			void setProjection(float left, float right, float bottom, float top);
		};
	}
}