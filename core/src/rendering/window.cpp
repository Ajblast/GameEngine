#include "gravpch.h"
#include "window.h"

#ifdef GRAV_PLATFORM_WINDOWS
	#include "platform/windows/windowsWindow.h"
#endif

namespace GRAVEngine
{
	namespace Rendering
	{
		window* window::create(const windowProperties& properties)
		{
		#ifdef GRAV_PLATFORM_WINDOWS
			return new windowsWindow(properties);
		#else
			GRAV_ASSERT(false);
		#endif
		}
	}
}