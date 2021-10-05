#pragma once

#include "common.h"
#include "camera.h"
#include "time/timestep.h"


namespace GRAVEngine
{
	namespace Rendering
	{
		class editorCamera : public camera
		{
		public:
			editorCamera() = default;
			editorCamera(float fov, float aspectRatio, float nearClip, float farClip);

			inline void setViewportSize(float width, float height) { m_ViewportWidth = width, m_ViewportHeight = height; recalculateProjectionMatrix(); }


		private:
			virtual void recalculateProjectionMatrix() override;

		private:
			float m_FOV = 45.0f;
		};
	}
}