#pragma once

#include "common.h"
#include <glm/glm.hpp>

namespace GRAVEngine
{
	namespace Rendering
	{
		class camera
		{
		public:
			camera() = default;
			camera(const glm::mat4& projection, const glm::mat4& view) : m_ProjectionMatrix(projection), m_ViewMatrix(view), m_ViewProjectionMatrix(projection * view) {}
			virtual ~camera() = default;


			const glm::vec3& getPosition() const { return m_Position; }
			void setPosition(const glm::vec3& position) { m_Position = position; recalculateViewMatrix(); }

			float getRotation() const { return m_Rotation; }
			void setRotation(float rotation) { m_Rotation = rotation; recalculateViewMatrix(); }


			const glm::mat4& getProjectionMatrix()		const { return m_ProjectionMatrix; }
			const glm::mat4& getViewMatrix()			const { return m_ViewMatrix; }
			const glm::mat4& getViewProjectionMatrix()	const { return m_ViewProjectionMatrix; }
		protected:
			void recalculateViewMatrix();
		protected:
			glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
			glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
			glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

			glm::vec3 m_Position = { 0.0f,0.0f , 0.0f };
			float m_Rotation = 0.0f;

		};
	}
}