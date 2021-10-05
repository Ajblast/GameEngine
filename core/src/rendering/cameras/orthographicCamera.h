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
			orthographicCamera(float left, float right, float bottom, float top, float nearClip, float farClip);

			void setProjection(float left, float right, float bottom, float top);
			void setProjection(float left, float right, float bottom, float top, float nearClip, float farClip);
		};
	}
}