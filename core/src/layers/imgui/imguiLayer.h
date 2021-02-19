#pragma once

#include "common.h"
#include "layers/layer.h"

namespace GRAVEngine
{
	namespace Layers
	{
		// Layer used to draw ImGui items. This is meant for debugging purposes.
		class imguiLayer : public layer
		{
		public:
			imguiLayer();
			~imguiLayer() = default;

			virtual void onAttach() override;
			virtual void onDetach() override;
			virtual void onEvent(Events::event& event) override;

			// Begin ImGui
			void begin();
			// End ImGui
			void end();

			// Declare whether events should be handled or not
			void blockEvents(bool block) { m_BlockEvents = block; }

			// Set the theme to dark colors
			void setDarkThemeColors();
		private:
			bool m_BlockEvents = true;
			float m_Time = 0.0f;
		};
	}
}