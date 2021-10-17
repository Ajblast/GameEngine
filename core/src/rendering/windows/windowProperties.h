#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace Rendering
	{
		// Properties for window creation
		struct GRAVAPI windowProperties
		{
		public:

			std::string m_Title;
			uint32 m_Width;
			uint32 m_Height;
			bool m_Visible;

			windowProperties(const std::string& title = "GRAVEngine", uint32 width = 1600, uint32 height = 900, bool visible = true) :
				m_Title(title), m_Width(width), m_Height(height), m_Visible(visible)
			{}
		};
	}
}