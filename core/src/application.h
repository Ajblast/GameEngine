#pragma once

#include "common.h"
#include "rendering/windows/window.h"
#include "events/event.h"
#include "events/eventDispatcher.h"
#include "events/applicationEvents.h"

#include "time/timestep.h"

#include "layers/layerStack.h"
#include "layers/imgui/imguiLayer.h"

#include "jobs/jobs.h"

#include <atomic>

int main(int argc, char** argv);

// TODO:Make it possible for a function to check what thread it is running in.
//		This would allow a debugging helper check that makes sure that functions that must run on the main thread are ran on the main thread

namespace GRAVEngine
{
	class GRAVAPI application
	{
	public:
		application(const std::string& name = "GRAVEngine Application");
		virtual ~application();

		// Event callback
		void onEvent(Events::event& event);

		// Push a layer onto the layer stack
		void pushLayer(Layers::layer* layer);
		// Push an overlay
		void pushOverlay(Layers::layer* overlay);
		// TODO: Potentially add functionality to pop layers and overlays

		// Get the current rendering window
		Rendering::window& getWindow() { return *m_Window; }

		// Close the application
		void close();

		// Get the ImGuiLayer
		Layers::imguiLayer* getImGuiLayer() { return m_ImGuiLayer; }

		// Get the application instance
		static application& getInstance() { return *s_Instance; }

	private:
		// Run the application
		void run();
		// Run worker thread where everything actually goes
		void runWorker();
		// Window close callback
		bool onWindowClose(Events::windowCloseEvent& event);
		// Window resize callback
		bool onWindowResize(Events::windowResizeEvent& event);

	private:
		scope<Rendering::window> m_Window;				// The window associated with the application
		scope<scope<Rendering::window>[]> m_JobWindows;	// Windows for jobs so that jobs will have render context

		std::atomic_bool m_NeedsRendering;		// Should the rendering occur
		std::atomic_bool m_Rendered;			// Should the rendering occur
		std::mutex m_RenderingMutex;			// Condition variable mutex
		std::condition_variable m_RenderingCV;	// Condition variable for waiting
		std::atomic_bool m_Running = true;		// Is the application running
		std::atomic_bool m_Minimized = false;	// Is the application minimized

		// Job Manager
		Jobs::jobManager m_JobManager;					// The job manager for the application
		Jobs::jobManagerOptions m_JobManagerOptions;	// Default job manager options

		float m_LastFrameTime = 0.0f;			// Time to run last frame

		Layers::layerStack m_LayerStack;		// The update layer stack
		Layers::imguiLayer* m_ImGuiLayer;		// The ImGuiLayer
	private:
		static application* s_Instance;			// Application instance

		// Allow the main function to call the private methods, such as run
		friend int ::main(int argc, char** argv);
	};

	// Defined in the client
	GRAVAPI application* createApplication();
}