#include "gravpch.h"
#include "application.h"

#include "io/input/input.h"
#include "io/input/inputManager.h"
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

	// Generate the windows for all of the jobmanager threads so that they have a context
	size_t jobThreadCount = m_JobManagerOptions.m_NumThreads;
	m_JobWindows = createScope<scope<Rendering::window>[]>(jobThreadCount);
	for (size_t i = 0; i < jobThreadCount; i++)
	{
		Rendering::windowProperties props("JobWindow: " + std::to_string(i));
		props.m_Visible = false;
		m_JobWindows[i] = std::move(Rendering::window::create(m_Window, props));
	}

	// Set the callback for when a thread is initialized so that they set their graphics context to current
	m_JobManagerOptions.m_Callbacks.m_OnThreadInitialized = [&](Jobs::threadIndex index) {
		m_JobWindows[index]->bindGraphicsContext();
	};

	// Rebind the main window's graphical context as creating the job window's unbinds it
	m_Window->bindGraphicsContext();
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
	GRAV_PROFILE_FUNCTION();

	m_Running.store(false, std::memory_order_release);
}

void GRAVEngine::application::run()
{
	GRAV_PROFILE_FUNCTION();

	// Create the worker running thread
	std::thread runThread(GRAV_BIND_EVENT_FN(runWorker));

	// This loop will run only on the main thread. Anything that must only be ran on the main thread goes here.
	// Have to have synchronization from the run thread
	while (m_Running.load(std::memory_order_acquire))
	{
		std::unique_lock<std::mutex> lk(m_RenderingMutex);

		// Start the rendering if needed
		if (m_NeedsRendering.load(std::memory_order_acquire))
		{
			Rendering::rendererCommand::setClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
			Rendering::rendererCommand::clear();
			GRAVEngine::Rendering::renderer2D::flush();

			// Imgui update
			m_ImGuiLayer->begin();
			{
				GRAV_PROFILE_SCOPE("LayerStack onImGuiRender");
				// For each layer, render ImGui
				for (Layers::layer* layer : m_LayerStack)
					layer->onImGuiRender();
			}
			m_ImGuiLayer->end();

			// Reset the rendering statistics
			GRAVEngine::Rendering::renderer2D::resetStats();

			// Poll for any events
			IO::inputManager::getInstance()->pollEvents();
			m_Window->onUpdate();

			// The rendering has finished
			m_NeedsRendering.store(false, std::memory_order_release);
			m_Rendered.store(true, std::memory_order_release);
			m_RenderingCV.notify_all();
		}
	}

	{
		std::unique_lock<std::mutex> lk(m_RenderingMutex);

		// Make sure to wake up the worker thread in case it is waiting to end
		m_Rendered.store(true, std::memory_order_release);
		m_RenderingCV.notify_all();
	}

	// Join the worker thred
	runThread.join();	
}
void GRAVEngine::application::runWorker()
{
	GRAV_PROFILE_FUNCTION();

	// Startup the job manager
	// This has to be done in the runWorker thread instead of the main thread, otherwise this thread wouldn't be accessible to the job system
	m_JobManager.startUp(m_JobManagerOptions);

	while (m_Running.load(std::memory_order_acquire))
	{
		// Get the delta time
		float time = (float)glfwGetTime();
		Time::timestep timestep = Time::timestep((float)(time - m_LastFrameTime));
		m_LastFrameTime = time;

		if (m_Minimized == false)
		{
			{
				GRAV_PROFILE_SCOPE("LayerStack onUpdate");
				// Update layers
				for (Layers::layer* layer : m_LayerStack)
					layer->onUpdate(timestep);
			}

			// Wait for rendering to finish
			{
				GRAV_PROFILE_SCOPE("LayerStack Rendering");
				// Wait for the main thread to finish rendering because of main thread glfw requirements
				std::unique_lock<std::mutex> lk(m_RenderingMutex);
				m_NeedsRendering.store(true, std::memory_order_release);

				// Wait until run has finished rendering
				m_RenderingCV.wait(lk, [&] {
					return m_Rendered.load(std::memory_order_acquire);
					});
			}
		}
	}

	// Shutdown the job manager
	m_JobManager.shutDown();
}

bool GRAVEngine::application::onWindowClose(Events::windowCloseEvent& event)
{
	GRAV_PROFILE_FUNCTION();

	m_Running.store(false, std::memory_order_release);

	// This event is now handled
	return true;
}
bool GRAVEngine::application::onWindowResize(Events::windowResizeEvent& event)
{
	GRAV_PROFILE_FUNCTION();

	if (event.getWidth() == 0 || event.getHeight() == 0)
	{
		m_Minimized.store(true, std::memory_order_release);
		return false;
	}

	m_Minimized.store(false, std::memory_order_release);
	Rendering::renderer::onWindowResize(event.getWidth(), event.getHeight());

	// Event is not handeled no matter what
	return false;
}
