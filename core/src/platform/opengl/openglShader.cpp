#include "gravpch.h"
#include "platform/opengl/openglShader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "io/textFile.h"
#include <array>

namespace GRAVEngine
{
	namespace Rendering
	{
		// Convert a fragment type string to a OpenGL enumeration
		GLenum shaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			// Unknown shader type
			GRAV_ASSERT(false);
			return 0;
		}
	}
}

GRAVEngine::Rendering::openglShader::openglShader(const std::string& filepath)
{
	GRAV_PROFILE_FUNCTION();

	std::string source = readFile(filepath);
	auto shaderSources = preProcess(source);
	compile(shaderSources);

	// Extract name from filepath
	auto lastSlash = filepath.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	auto lastDot = filepath.rfind('.');
	auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
	m_Name = filepath.substr(lastSlash, count);
}
GRAVEngine::Rendering::openglShader::openglShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) :
	m_Name(name)
{
	GRAV_PROFILE_FUNCTION();

	std::unordered_map<GLenum, std::string> sources;
	sources[GL_VERTEX_SHADER] = vertexSrc;
	sources[GL_FRAGMENT_SHADER] = fragmentSrc;
	compile(sources);
}

GRAVEngine::Rendering::openglShader::~openglShader()
{
	GRAV_PROFILE_FUNCTION();

	// Delete this shader from the GPU
	glDeleteProgram(m_RendererID);
}

void GRAVEngine::Rendering::openglShader::bind() const
{
	GRAV_PROFILE_FUNCTION();

	// Use the shader
	glUseProgram(m_RendererID);
}
void GRAVEngine::Rendering::openglShader::unbind() const
{
	GRAV_PROFILE_FUNCTION();

	// Use null shader
	glUseProgram(m_RendererID);
}

void GRAVEngine::Rendering::openglShader::setInt(const std::string& name, int32 value)
{
	GRAV_PROFILE_FUNCTION();

	uploadUniformInt(name, value);
}
void GRAVEngine::Rendering::openglShader::setIntArray(const std::string& name, int32* values, uint32 count)
{
	GRAV_PROFILE_FUNCTION();

	uploadUniformIntArray(name, values, count);
}
void GRAVEngine::Rendering::openglShader::setFloat(const std::string& name, float value)
{
	GRAV_PROFILE_FUNCTION();

	uploadUniformFloat(name, value);
}
void GRAVEngine::Rendering::openglShader::setFloat2(const std::string& name, const glm::vec2& value)
{
	GRAV_PROFILE_FUNCTION();

	uploadUniformFloat2(name, value);
}
void GRAVEngine::Rendering::openglShader::setFloat3(const std::string& name, const glm::vec3& value)
{
	GRAV_PROFILE_FUNCTION();

	uploadUniformFloat3(name, value);
}
void GRAVEngine::Rendering::openglShader::setFloat4(const std::string& name, const glm::vec4& value)
{
	GRAV_PROFILE_FUNCTION();

	uploadUniformFloat4(name, value);
}
void GRAVEngine::Rendering::openglShader::setMat4(const std::string& name, const glm::mat4& value)
{
	GRAV_PROFILE_FUNCTION();

	uploadUniformMat4(name, value);
}

void GRAVEngine::Rendering::openglShader::uploadUniformInt(const std::string& name, int value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1i(location, value);
}
void GRAVEngine::Rendering::openglShader::uploadUniformIntArray(const std::string& name, int* values, uint32 count)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1iv(location, count, values);
}
void GRAVEngine::Rendering::openglShader::uploadUniformFloat(const std::string& name, float value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1f(location, value);
}
void GRAVEngine::Rendering::openglShader::uploadUniformFloat2(const std::string& name, const glm::vec2& value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform2f(location, value.x, value.y);
}
void GRAVEngine::Rendering::openglShader::uploadUniformFloat3(const std::string& name, const glm::vec3& value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}
void GRAVEngine::Rendering::openglShader::uploadUniformFloat4(const std::string& name, const glm::vec4& value)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}
void GRAVEngine::Rendering::openglShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
void GRAVEngine::Rendering::openglShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string GRAVEngine::Rendering::openglShader::readFile(const std::string& filepath)
{
	GRAV_PROFILE_FUNCTION();

	std::string result;
	try
	{
		IO::textFile file(filepath.c_str(), IO::fileMode::INPUT, false);

		size_t size = file.fileSize();

		if (size > 0)
		{
			result.resize(file.fileSize());
			file.read(&result[0], size);
		}
		else
			// Empty file
			GRAV_ASSERT(false);
	}
	catch (Exceptions::IO::fileNotFoundException& e)
	{
		// Log the exception
		GRAV_LOG_CRITICAL(e.what());

		// Unable to open file
		GRAV_ASSERT(false);
	}
	catch (Exceptions::IO::unauthorizedAccessException& e)
	{
		// Log the exception
		GRAV_LOG_CRITICAL(e.what());
		
		// Unable to read file
		GRAV_ASSERT(false);
	}
	catch (Exceptions::IO::ioException& e)
	{
		// Log the exception
		GRAV_LOG_CRITICAL(e.what());
		
		// Unknown IO exception
		GRAV_ASSERT(false);
	}

	return result;
}

std::unordered_map<GLenum, std::string> GRAVEngine::Rendering::openglShader::preProcess(const std::string& source)
{
	GRAV_PROFILE_FUNCTION();

	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";			// The token before defining the type the next section of shader code is
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);		// Start of shader type declaration line

	// Continue until the end of string
	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos);	// End of shader type declaration line
		GRAV_ASSERT(eol != std::string::npos);			// Syntax error

		size_t begin = pos + typeTokenLength + 1;		// Start of shader type name (after "#type keyword)
		std::string type = source.substr(begin, eol - begin);	// Get the type specified after the token
		GRAV_ASSERT(shaderTypeFromString(type));		// Invalid shader type specified

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);	// Start of shader code after shader type declaration line
		GRAV_ASSERT(nextLinePos != std::string::npos);	// Syntax error
		pos = source.find(typeToken, nextLinePos);

		shaderSources[shaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
	}

	return shaderSources;
}

void GRAVEngine::Rendering::openglShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
{
	GRAV_PROFILE_FUNCTION();

	// Create a rendering program
	GLuint program = glCreateProgram();
	GRAV_ASSERT(shaderSources.size() <= 2);	// Only two shaders currently supported

#pragma region ShaderCompilation
	std::array<GLenum, 2> glShaderIDs;
	int glShaderIDIndex = 0;

	// Create and compile each shader
	for (auto& kv : shaderSources)
	{
		GLenum type = kv.first;
		const std::string& source = kv.second;

		// Create a shader on the GPU based on its type
		GLuint shader = glCreateShader(type);

		// Set the shader's source information
		const GLchar* sourceCStr = source.c_str();
		glShaderSource(shader, 1, &sourceCStr, 0);

		// Compile the shader
		glCompileShader(shader);

		GLint isCompiled = 0;
		// Get the compile status of the shader
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			// Get the logging error of compiling the shader
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// Get the logging error
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			// Deletet eh shader from the GPU
			glDeleteShader(shader);

			// Log the error
			GRAV_LOG_LINE_ERROR("%s", infoLog.data());
			GRAV_ASSERT(false); // Shader compilation failure
			break;
		}

		// Attach this shader to the program
		glAttachShader(program, shader);
		glShaderIDs[glShaderIDIndex++] = shader;
	}

#pragma endregion


	// Set the id of this shader
	m_RendererID = program;

#pragma region PrgramCompilation
	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	// Get the linking status
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		// Get the length of the logging error
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);

		// Delete every shader attached to the program
		for (auto id : glShaderIDs)
			glDeleteShader(id);

		// Log that the error occured
		GRAV_LOG_LINE_ERROR("%s", infoLog.data());
		GRAV_ASSERT(false);	// Shader link failure!
		return;
	}
#pragma endregion

	// Delete shaders after linking. Shaders are not needed as they are now linked into the program
	for (auto id : glShaderIDs)
	{
		// Shaders have to be detached first
		glDetachShader(program, id);
		glDeleteShader(id);
	}
}
