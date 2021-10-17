#pragma once

#include "common.h"
#include "windowProperties.h"
#include "events/event.h"
#include "rendering/graphicsContext.h"

#include <functional>

namespace GRAVEngine
{
	namespace Rendering
	{
		// Interface representing a desktop system based window
		class GRAVAPI window
		{
		public:
			using eventCallbackFunction = std::function<void(Events::event&)>;

			virtual ~window() {}

			virtual void onUpdate() = 0;

			virtual uint32 getWidth() const = 0;
			virtual uint32 getHeight() const = 0;

			// Window Attributes
			virtual void setEventCallback(const eventCallbackFunction& callback) = 0;
			virtual void setVSync(bool enabled) = 0;
			virtual bool isVSync() const = 0;

			// Get the native representation of this window
			virtual void* getNativeWindow() const = 0;

			// Bind this window's graphical context
			void bindGraphicsContext();
			// Unbind this window's graphical context
			void unbindGraphicsContext();

			// Create a window. This must be ran on the main thread!
			static scope<window> create(const windowProperties& properties = windowProperties());
			// Create a dependent window that shares resources with another window. This must be ran on the main thread!
			static scope<window> create(scope<window>& window, const windowProperties& properties = windowProperties());
		protected:
			scope<graphicsContext> m_Context;
		};
	}
}