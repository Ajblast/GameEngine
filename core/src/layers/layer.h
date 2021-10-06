#pragma once

#include "common.h"
#include "time/timestep.h"
#include "events/event.h"

#include <string>

namespace GRAVEngine
{
	namespace Layers
	{
		// A layer represents a collection of items, whether that be rendering, entities, or anything else
		class GRAVAPI layer
		{
		public:
			layer(const std::string& name = "Layer");
			virtual ~layer() = default;

			virtual void onAttach() {}
			virtual void onDetach() {}
			virtual void onUpdate(Time::timestep ts) {}
			virtual void onImGuiRender() {}
			virtual void onEvent(Events::event& event) {}

			const std::string& getName() const { return m_DebugName; }

		protected:
			std::string m_DebugName;
		};
	}
}