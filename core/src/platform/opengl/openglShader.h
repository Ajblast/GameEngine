#pragma once

#include "common.h"
#include "rendering/shaders/shader.h"
#include <glm/glm.hpp>

namespace GRAVEngine
{
	namespace Rendering
	{
		// TODO: Remove this

		typedef unsigned int GLenum;

		// An OpenGL Shader
		class openglShader : public shader
		{
		public:
			openglShader(const std::string& filepath);
			openglShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
			virtual ~openglShader();

			// Bind a shader
			virtual void bind() const override;
			// Unbind a shader
			virtual void unbind() const override;

			// Set named integer in the shader
			virtual void setInt(const std::string& name, int32 value) override;
			// Set named integer array in the shader
			virtual void setIntArray(const std::string& name, int32* values, uint32 count) override;
			// Set named float in the shader
			virtual void setFloat(const std::string& name, float value) override;
			// Set named float in the shader
			virtual void setFloat2(const std::string& name, const glm::vec2& value) override;
			// Set named float3 vector in the shader
			virtual void setFloat3(const std::string& name, const glm::vec3& value) override;
			// Set named float4 vector in the shader
			virtual void setFloat4(const std::string& name, const glm::vec4& value) override;
			// Set named mat4 in the shader
			virtual void setMat4(const std::string& name, const glm::mat4& value) override;

			void uploadUniformInt(const std::string& name, int value);
			void uploadUniformIntArray(const std::string& name, int* values, uint32 count);

			void uploadUniformFloat(const std::string& name, float value);
			void uploadUniformFloat2(const std::string& name, const glm::vec2& value);
			void uploadUniformFloat3(const std::string& name, const glm::vec3& value);
			void uploadUniformFloat4(const std::string& name, const glm::vec4& value);

			void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
			void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);



			// Get the name of this shader
			virtual const std::string& getName() const override { return m_Name; }
		private:
			// TODO: Remove the file reading and abstract it to a different system. The shader file shouldn't do this functionality
			// Read a file and get its contents
			std::string readFile(const std::string& filepath);
			// Process a source string and get the vertex and fragment sections of it
			std::unordered_map<GLenum, std::string> preProcess(const std::string& source);

			// Compile a shader from a source
			void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		private:
			uint32 m_RendererID;
			std::string m_Name;
		};
	}
}