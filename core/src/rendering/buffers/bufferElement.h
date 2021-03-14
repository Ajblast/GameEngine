#pragma once

#include "common.h"
#include "rendering/shaders/shaderDataType.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		// An element is a buffer Layout
		struct bufferElement
		{
			std::string m_Name;		// Name of the element
			shaderDataType m_Type;	// Elment type
			uint32 m_Size;			// Element size
			size_t m_Offset;		// Offset into buffer layout
			bool m_Normalized;		// Is the element normalized

			bufferElement() = default;
			bufferElement(shaderDataType type, const std::string& name, bool normalized = false) :
				m_Name(name), m_Type(type), m_Size(shaderDataTypeSize(type)), m_Offset(0), m_Normalized(normalized)
			{}

			// Get the number of items this element is
			uint32 getComponentCount() const
			{
				switch (m_Type)
				{
				case GRAVEngine::Rendering::shaderDataType::FLOAT:	return 1;
				case GRAVEngine::Rendering::shaderDataType::FLOAT2:	return 2;
				case GRAVEngine::Rendering::shaderDataType::FLOAT3:	return 3;
				case GRAVEngine::Rendering::shaderDataType::FLOAT4:	return 4;
				case GRAVEngine::Rendering::shaderDataType::MAT3:	return 3; // 3* float3
				case GRAVEngine::Rendering::shaderDataType::MAT4:	return 4; // 4* float4
				case GRAVEngine::Rendering::shaderDataType::INT:	return 1;
				case GRAVEngine::Rendering::shaderDataType::INT2:	return 2;
				case GRAVEngine::Rendering::shaderDataType::INT3:	return 3;
				case GRAVEngine::Rendering::shaderDataType::INT4:	return 4;
				case GRAVEngine::Rendering::shaderDataType::BOOL:	return 1;
				}

				// Unknown shader data type
				GRAV_ASSERT(false);
				return 0;
			}
		};
	}
}