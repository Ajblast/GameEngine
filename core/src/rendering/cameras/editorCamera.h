#pragma once

#include "common.h"
#include "camera.h"
#include "time/timestep.h"
#include "events/event.h"
#include "events/eventDispatcher.h"
#include "events/mouseEvents.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		class editorCamera : public camera
		{
		public:
			editorCamera() = default;
			editorCamera(float fov, float aspectRatio, float nearClip, float farClip);

			void OnUpdate(Time::timestep timestep);
			void OnEvent(Events::event& event);

			inline float getDistance() const { return m_Distance; }
			inline void setDistance(float distance) { m_Distance = distance; }

			inline void setViewportSize(float width, float height) { m_ViewportWidth = width, m_ViewportHeight = height; updateProjection(); }

			glm::vec3 up() const;
			glm::vec3 right() const;
			glm::vec3 forward() const;
			glm::quat getOrientation() const;

			float getPitch() const { return m_Pitch; }
			float getYaw() const { return m_Yaw; }
		private:
			void updateProjection();
			void updateView();

			bool onMouseScroll(Events::mouseScrolledEvent& event);

			void mousePan(const glm::vec2& delta);
			void mouseRotate(const glm::vec2& delta);
			void mouseZoom(float delta);

			glm::vec3 calculatePosition() const;

			std::pair<float, float> panSpeed() const;
			float rotationSpeed() const;
			float zoomSpeed() const;

		private:
			float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

			glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

			glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

			float m_Distance = 10.0f;
			float m_Pitch = 0.0f, m_Yaw = 0.0f;

			float m_ViewportWidth = 1280, m_ViewportHeight = 720;
		};
	}
}