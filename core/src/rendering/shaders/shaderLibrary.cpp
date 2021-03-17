#include "gravpch.h"
#include "rendering/shaders/shaderLibrary.h"
#include "rendering/renderer/rendererAPI.h"

void GRAVEngine::Rendering::shaderLibrary::add(const std::string& name, const ref<shader>& shader)
{
	GRAV_ASSERT(exists(name) == false);

	m_Shaders[name] = shader;
}
void GRAVEngine::Rendering::shaderLibrary::add(const ref<shader>& shader)
{
	// Get the shader's name and add it to the library
	const std::string& name = shader->getName();
	add(name, shader);
}

GRAVEngine::ref<GRAVEngine::Rendering::shader> GRAVEngine::Rendering::shaderLibrary::load(const std::string& filepath)
{
	// Create the shader
	auto shader = rendererAPI::getInstance().createShader(filepath);
	add(shader);
	return shader;
}
GRAVEngine::ref<GRAVEngine::Rendering::shader> GRAVEngine::Rendering::shaderLibrary::load(const std::string& name, const std::string& filepath)
{
	// Create the shader
	auto shader = rendererAPI::getInstance().createShader(filepath);
	add(name, shader);
	return shader;
}

GRAVEngine::ref<GRAVEngine::Rendering::shader> GRAVEngine::Rendering::shaderLibrary::get(const std::string& name)
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


