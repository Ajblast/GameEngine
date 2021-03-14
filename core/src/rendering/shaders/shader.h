#pragma once

#include "common.h"
#include <glm/glm.hpp>

namespace GRAVEngine
{
	namespace Rendering
	{
		// Generic shader interface
		class shader
		{
		public:
			virtual ~shader() = default;

			virtual void bind() const = 0;		// Bind this shader to the GPU
			virtual void unbind() const = 0;	// Unbind this shader from the GPU

			// Set named integer in the shader
			virtual void setInt(const std::string& name, int32 value) = 0;
			// Set named integer array in the shader
			virtual void setIntArray(const std::string& name, int32* values, uint32 count) = 0;
			// Set named float in the shader
			virtual void setFloat(const std::string& name, float value) = 0;
			// Set named float in the shader
			virtual void setFloat2(const std::string& name, const glm::vec2& value) = 0;
			// Set named float3 vector in the shader
			virtual void setFloat3(const std::string& name, const glm::vec3& value) = 0;
			// Set named float4 vector in the shader
			virtual void setFloat4(const std::string& name, const glm::vec4& value) = 0;
			// Set named mat4 in the shader
			virtual void setMat4(const std::string& name, const glm::mat4& value) = 0;

			// Get the name of this shader
			virtual const std::string& getName() const = 0;

			//// Create a shader from a file
			//static shader* create(const std::string& filePath);
			//// Create a shader with a vertex and fragment string
			//static shader* create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		};
	}
}