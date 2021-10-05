#include "gravpch.h"
#include "application.h"

#include "io/input.h"
#include "rendering/renderer/renderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

GRAVEngine::application* GRAVEngine::application::s_Instance = nullptr;

GRAVEngine::application::application(const std::string& name)
{
	GRAV_ASSERT(s_Instance == nullptr);
	GRAV_PROFILE_FUNCTION();

	s_Instance = this;

	// Create the starting window
	m_Window = Rendering::window::create(Rendering::windowProperties(name));
	m_Window->setEventCallback(GRAV_BIND_EVENT_FN(application::onEvent));

	// Startup the rendering
	Rendering::renderer::startup();

	 m_ImGuiLayer = new Layers::imguiLayer();
	 pushOverlay(m_ImGuiLayer);

	 // Start the AI trainer
}

GRAVEngine::application::~application()
{
	GRAV_PROFILE_FUNCTION();

	// TODO: Shutdown the renderer
	Rendering::renderer::shutdown();
}

void GRAVEngine::application::onEvent(Events::event& event)
{
	GRAV_PROFILE_FUNCTION();

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
	GRAV_PROFILE_FUNCTION();

	// Push the layer
	m_LayerStack.pushLayer(layer);
	// Tell the layer that it is now attached
	layer->onAttach();
}

void GRAVEngine::application::pushOverlay(Layers::layer* overlay)
{
	GRAV_PROFILE_FUNCTION();

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
	GRAV_PROFILE_FUNCTION();

	while (m_Running)
	{
		GRAV_PROFILE_SCOPE("RunLoop");

		// Get the delta time
		float time = (float)glfwGetTime();
		Time::timestep timestep = Time::timestep(time - m_LastFrameTime);
		m_LastFrameTime = time;

		if (m_Minimized == false)
		{
			{
				GRAV_PROFILE_SCOPE("LayerStack onUpdate");
				// Update layers
				for (Layers::layer* layer : m_LayerStack)
					layer->onUpdate(timestep);
			}

			// Start updating the ImGuiLayer
			m_ImGuiLayer->begin();
			{
				GRAV_PROFILE_SCOPE("LayerStack onImGuiRender");
				// For each layer, render ImGui
				for (Layers::layer* layer : m_LayerStack)
					layer->onImGuiRender();
			}
			// End updating the ImGuiLayer
			m_ImGuiLayer->end();
		}

		// TODO: Update the window
		m_Window->onUpdate();
	}
}

bool GRAVEngine::application::onWindowClose(Events::windowCloseEvent& event)
{
	m_Running = false;

	// This event is now handled
	return true;
}

bool GRAVEngine::application::onWindowResize(Events::windowResizeEvent& event)
{
	GRAV_PROFILE_FUNCTION();

	if (event.getWidth() == 0 || event.getHeight() == 0)
	{
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;
	Rendering::renderer::onWindowResize(event.getWidth(), event.getHeight());

	// Event is not handeled no matter what
	return false;
}
