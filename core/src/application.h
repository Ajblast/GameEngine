#pragma once

#include "common.h"
#include "rendering/window.h"
#include "events/event.h"
#include "events/eventDispatcher.h"
#include "events/applicationEvents.h"

int main(int argc, char** argv);

namespace GRAVEngine
{
	class application
	{
	public:
		application(const std::string& name = "GRAVEngine Application");
		virtual ~application();

		void onEvent(Events::event& event);

		Rendering::window& getWindow() { return *m_Window; }

		void close();

		static application& getInstance() { return *s_Instance; }

	private:
		void run();
		bool onWindowClose(Events::windowCloseEvent& event);
		bool onWindowResize(Events::windowResizeEvent& event);

	private:
		Rendering::window* m_Window;

		bool m_Running = true;
		bool m_Minimized = false;

		float m_LastFrameTime = 0.0f;
	private:
		static application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// Defined in the client
	application* createApplication();
}