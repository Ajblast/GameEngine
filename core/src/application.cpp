#include "gravpch.h"
#include "application.h"

#include <GLFW/glfw3.h>
#include "io/input.h"
#include <glad/glad.h>

GRAVEngine::application* GRAVEngine::application::s_Instance = nullptr;

GRAVEngine::application::application(const std::string& name)
{
	GRAV_ASSERT(s_Instance == nullptr);
	s_Instance = this;

	// Create the starting window
	m_Window = Rendering::window::create(Rendering::windowProperties(name));
	m_Window->setEventCallback(GRAV_BIND_EVENT_FN(application::onEvent));

	// TODO: Initialize the renderer

	 m_ImGuiLayer = new Layers::imguiLayer();
	 pushOverlay(m_ImGuiLayer);
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

	// Reverse iterate over the layerstack to pass events
	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
	{
		// Stop iterating if the event has been handled
		if (event.m_Handled)
			break;
		// Send the event
		(*it)->onEvent(event);
	}
}

void GRAVEngine::application::pushLayer(Layers::layer* layer)
{
	// Push the layer
	m_LayerStack.pushLayer(layer);
	// Tell the layer that it is now attached
	layer->onAttach();
}

void GRAVEngine::application::pushOverlay(Layers::layer* overlay)
{
	// Push the overlay
	m_LayerStack.pushOverlay(overlay);
	// Tell the overlay that it is now attached
	overlay->onAttach();
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
		// Get the delta time
		float time = (float)glfwGetTime();
		Time::timestep timestep = Time::timestep(time - m_LastFrameTime);
		m_LastFrameTime = time;

		if (m_Minimized == false)
		{
			// Update layers
			for (Layers::layer* layer : m_LayerStack)
				layer->onUpdate(timestep);

			// Start updating the ImGuiLayer
			m_ImGuiLayer->begin();
			// For each layer, render ImGui
			for (Layers::layer* layer : m_LayerStack)
				layer->onImGuiRender();
			// End updating the ImGuiLayer
			m_ImGuiLayer->end();


			// TODO: Critical move this io key pressed event into the sandbox application

			// Test code for colors
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);


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
