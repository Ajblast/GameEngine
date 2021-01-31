#include "gravpch.h"
#include "application.h"

#include <GLFW/glfw3.h>

#include "io/input.h"

GRAVEngine::application* GRAVEngine::application::s_Instance = nullptr;

GRAVEngine::application::application(const std::string& name)
{
	GRAV_ASSERT(s_Instance == nullptr);
	s_Instance = this;

	// Create the starting window
	m_Window = Rendering::window::create(Rendering::windowProperties(name));
	m_Window->setEventCallback(GRAV_BIND_EVENT_FN(application::onEvent));

	// TODO: Initialize the renderer


}

GRAVEngine::application::~application()
{
	// TODO: Shutdown the renderer
}

void GRAVEngine::application::onEvent(Events::event& event)
{
	// Dispatch an event
	Events::eventDispatcher dispatcher(event);
	dispatcher.dispatch<Events::windowCloseEvent>(GRAV_BIND_EVENT_FN(application::onWindowClose));
	dispatcher.dispatch<Events::windowResizeEvent>(GRAV_BIND_EVENT_FN(application::onWindowResize));

	// TODO: For every layer, send an event to it
}

void GRAVEngine::application::close()
{
	m_Running = false;
}

void GRAVEngine::application::run()
{
	GRAV_LOG_LINE_INFO("%s: Run Application", __FUNCTION__);

	while (m_Running)
	{
		// TODO: Update delta time

		if (m_Minimized == false)
		{
			// TODO: Update every layer

			// ImGUI

			if (IO::Input::isKeyPressed(Keys::Space))
			{
				GRAV_LOG_LINE_INFO("%s: Space Pressed!!!", __FUNCTION__);
				close();
			}
		}

		// TODO: Update the window
		m_Window->onUpdate();
	}
}

bool GRAVEngine::application::onWindowClose(Events::windowCloseEvent& event)
{
	GRAV_LOG_LINE_INFO("%s: Close Window", __FUNCTION__);
	m_Running = false;

	// This event is now handled
	return true;
}

bool GRAVEngine::application::onWindowResize(Events::windowResizeEvent& event)
{
	//GRAV_LOG_LINE_INFO("%s: Resize Window", __FUNCTION__);

	if (event.getWidth() == 0 || event.getHeight() == 0)
	{
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;
	// TODO: Tell renderer that the window resized

	// Event is not handeled no matter what
	return false;
}
