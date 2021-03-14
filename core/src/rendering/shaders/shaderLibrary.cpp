#include "gravpch.h"
#include "rendering/shaders/shaderLibrary.h"
#include "rendering/renderer/rendererAPI.h"

void GRAVEngine::Rendering::shaderLibrary::add(const std::string& name, shader* shader)
{
	GRAV_ASSERT(exists(name) == false);

	m_Shaders[name] = shader;
}
void GRAVEngine::Rendering::shaderLibrary::add(shader* shader)
{
	// Get the shader's name and add it to the library
	const std::string& name = shader->getName();
	add(name, shader);
}

GRAVEngine::Rendering::shader* GRAVEngine::Rendering::shaderLibrary::load(const std::string& filepath)
{
	// Create the shader
	GRAV_ASSERT(rendererAPI::getInstance());	// The renderer API must be made beforehand
	auto shader = rendererAPI::getInstance()->createShader(filepath);
	add(shader);
	return shader;
}
GRAVEngine::Rendering::shader* GRAVEngine::Rendering::shaderLibrary::load(const std::string& name, const std::string& filepath)
{
	// Create the shader
	GRAV_ASSERT(rendererAPI::getInstance());	// The renderer API must be made beforehand
	auto shader = rendererAPI::getInstance()->createShader(filepath);
	add(name, shader);
	return shader;
}

GRAVEngine::Rendering::shader* GRAVEngine::Rendering::shaderLibrary::get(const std::string& name)
{
	// Ensure that this shader exists
	GRAV_ASSERT(exists(name));
	return m_Shaders[name];
}

bool GRAVEngine::Rendering::shaderLibrary::exists(const std::string& name) const
{
	// Find the shader and see if it exists
	return m_Shaders.find(name) != m_Shaders.end();
}


