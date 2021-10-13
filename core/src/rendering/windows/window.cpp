#include "gravpch.h"
#include "window.h"

#ifdef GRAV_PLATFORM_WINDOWS
	#include "platform/windows/windowsWindow.h"
#endif

namespace GRAVEngine
{
	namespace Rendering
	{
		void window::bindGraphicsContext()
		{
			m_Context->bind();
		}
		void window::unbindGraphicsContext()
		{
			m_Context->unbind();
		}

		scope<window> window::create(const windowProperties& properties)
		{
		#ifdef GRAV_PLATFORM_WINDOWS
			return createScope<windowsWindow>(properties);
		#else
			GRAV_ASSERT(false);
		#endif
		}
		scope<window> window::create(scope<window>& window, const windowProperties& properties)
		{
			#ifdef GRAV_PLATFORM_WINDOWS
			return createScope<windowsWindow>((scope<windowsWindow>&) window, properties);
			#else
			GRAV_ASSERT(false);
			#endif
		}
	}
}