#include "gravpch.h"
#include "window.h"

#ifdef GRAV_PLATFORM_WINDOWS
	#include "platform/windows/windowsWindow.h"
#endif

namespace GRAVEngine
{
	namespace Rendering
	{
		scope<window> window::create(const windowProperties& properties)
		{
		#ifdef GRAV_PLATFORM_WINDOWS
			return createScope<windowsWindow>(properties);
		#else
			GRAV_ASSERT(false);
		#endif
		}
	}
}