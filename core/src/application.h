#pragma once

#include "common.h"
#include "rendering/window.h"
#include "events/event.h"
#include "events/eventDispatcher.h"
#include "events/applicationEvents.h"
#include "time/timestep.h"
#include "layers/layerStack.h"
#include "layers/imgui/imguiLayer.h"

int main(int argc, char** argv);

namespace GRAVEngine
{
	class application
	{
	public:
		application(const std::string& name = "GRAVEngine Application");
		virtual ~application();

		// Event callback
		void onEvent(Events::event& event);

		void pushLayer(Layers::layer* layer);
		void pushOverlay(Layers::layer* overlay);
		// TODO: Potentially add functionality to pop layers and overlays

		// Get the current rendering window
		Rendering::window& getWindow() { return *m_Window; }

		// Close the application
		void close();

		// Get the application instance
		static application& getInstance() { return *s_Instance; }

	private:
		// Run the application
		void run();
		// Window close callback
		bool onWindowClose(Events::windowCloseEvent& event);
		// Window resize callback
		bool onWindowResize(Events::windowResizeEvent& event);

	private:
		Rendering::window* m_Window;

		bool m_Running = true;
		bool m_Minimized = false;

		float m_LastFrameTime = 0.0f;

		Layers::layerStack m_LayerStack;
		Layers::imguiLayer* m_ImGuiLayer;
	private:
		static application* s_Instance;
		// Allow the main function to call the private methods, such as run
		friend int ::main(int argc, char** argv);
	};

	// Defined in the client
	application* createApplication();
}