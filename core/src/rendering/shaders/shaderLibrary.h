#pragma once

#include "common.h"
#include <string>
#include <unordered_map>

#include "rendering/shaders/shader.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		// Simple library to hold shaders
		class shaderLibrary
		{
		public:
			// Add a shader to this library
			void add(const std::string& name, const ref<shader>& shader);
			// Add a shader to this library
			void add(const ref<shader>& shader);

			// Create a shader from the file path and load it into the library
			ref<shader> load(const std::string& filepath);
			// Create a shader from the file path and load it into the library
			ref<shader> load(const std::string& name, const std::string& filepath);

			ref<shader> get(const std::string& name);

			bool exists(const std::string& name) const;
		private:
			// TODO: Change to allow memory management
			std::unordered_map<std::string, ref<shader>> m_Shaders;
		};
	}
}