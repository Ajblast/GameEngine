#pragma once

#include "common.h"
#include "events/event.h"
#include "events/eventDispatcher.h"
#include "events/mouseEvents.h"
#include "time/timestep.h"
#include <glm/glm.hpp>

namespace GRAVEngine
{
	namespace Rendering
	{
		class camera
		{
		public:
			camera() = default;
			camera(const glm::mat4& projection, const glm::mat4& view, float aspectRatio, float nearClip, float farClip);
			virtual ~camera() = default;

			void onEvent(Events::event& event);
			void OnUpdate(Time::timestep timestep);

			const glm::vec3& getPosition() const { return m_Position; }
			void setPosition(const glm::vec3& position) { m_FocalPoint = position; recalculateViewMatrix(); }

			const glm::vec3& getFocalPoint() const { return m_FocalPoint; }

			const glm::vec3& getRotation() const { return m_Rotation; }
			void setRotation(const glm::vec3& rotation) { m_Rotation = rotation; recalculateViewMatrix(); }

			float getPitch() const { return m_Rotation.x; }
			float getYaw() const { return m_Rotation.y; }
			float getRoll() const { return m_Rotation.z; }

			glm::vec3 up() const;
			glm::vec3 right() const;
			glm::vec3 forward() const;
			glm::quat getOrientation() const;

			const glm::mat4& getProjectionMatrix()		const { return m_ProjectionMatrix; }
			const glm::mat4& getViewMatrix()			const { return m_ViewMatrix; }
			const glm::mat4& getViewProjectionMatrix()	const { return m_ViewProjectionMatrix; }

		private:
			bool onMouseScroll(Events::mouseScrolledEvent& event);

			glm::vec3 calculatePosition() const;

			std::pair<float, float> panSpeed() const;
			float rotationSpeed() const;
			float zoomSpeed() const;

		protected:
			virtual void recalculateViewMatrix();
			virtual void recalculateProjectionMatrix();
			virtual void recalculateViewProjectionMatrix();

			void mousePan(const glm::vec2& delta);
			void mouseRotate(const glm::vec2& delta);
			void mouseZoom(float delta);
		protected:
			glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
			glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
			glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

			glm::vec3 m_Position = { 0.0f, 0.0f , 0.0f };
			glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };

			glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
			glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

			float m_Distance = 10.0f;

		protected:
			float m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;
			float m_ViewportWidth = 1280, m_ViewportHeight = 720;

		};
	}
}