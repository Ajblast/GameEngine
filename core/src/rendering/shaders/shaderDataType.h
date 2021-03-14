#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		enum class shaderDataType
		{
			NONE = 0, FLOAT, FLOAT2, FLOAT3, FLOAT4, MAT3, MAT4, INT, INT2, INT3, INT4, BOOL
		};

		static uint32 shaderDataTypeSize(shaderDataType type)
		{
			switch (type)
			{
			case GRAVEngine::Rendering::shaderDataType::FLOAT:	return 4;
			case GRAVEngine::Rendering::shaderDataType::FLOAT2:	return 4 * 2;
			case GRAVEngine::Rendering::shaderDataType::FLOAT3:	return 4 * 3;
			case GRAVEngine::Rendering::shaderDataType::FLOAT4:	return 4 * 4;
			case GRAVEngine::Rendering::shaderDataType::MAT3:	return 4 * 3 * 3;
			case GRAVEngine::Rendering::shaderDataType::MAT4:	return 4 * 4 * 4;
			case GRAVEngine::Rendering::shaderDataType::INT:	return 4;
			case GRAVEngine::Rendering::shaderDataType::INT2:	return 4 * 2;
			case GRAVEngine::Rendering::shaderDataType::INT3:	return 4 * 3;
			case GRAVEngine::Rendering::shaderDataType::INT4:	return 4 * 4;
			case GRAVEngine::Rendering::shaderDataType::BOOL:	return 1;
			}

			// Unknown Shader Data Type
			GRAV_ASSERT(false);
			return 0;
		}
	}
}