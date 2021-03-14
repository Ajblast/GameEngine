#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		struct windowProperties
		{
		public:

			std::string m_Title;
			uint32 m_Width;
			uint32 m_Height;

			windowProperties(const std::string& title = "GRAVEngine", uint32 width = 1600, uint32 height = 900) :
				m_Title(title), m_Width(width), m_Height(height)
			{}
		};
	}
}