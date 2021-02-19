#pragma once

#include "common.h"
#include "layer.h"

#include <vector>

namespace GRAVEngine
{
	namespace Layers
	{
		// Stack to hold layers. Layers are updated in forward order, but recieve events (like mouse events) in reverse order.
		// Overlays are placed in the back of the stack.
		class layerStack
		{
		public:
			// TODO: Allow recreation of the layer stack.
			// TODO: Change how the ownership of layers is handled. Potential Memory Leak
			//	Layers are deleted as long as they are in the stack when the layer stack is deallocated.
			//	The layer stack needs to take full ownership and should delete layers when they are poped. Currently the ownership is lost. Potential memory leak

			layerStack() = default;
			~layerStack();

			// Push a layer
			void pushLayer(layer* layer);
			// Push an overlay
			void pushOverlay(layer* overlay);
			// Pop a layer
			void popLayer(layer* layer);
			// Pop an overlay
			void popOverlay(layer* overlay);

			// Iterators
			std::vector<layer*>::iterator begin() { return m_Layers.begin(); }
			std::vector<layer*>::iterator end() { return m_Layers.end(); }
			std::vector<layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
			std::vector<layer*>::reverse_iterator rend() { return m_Layers.rend(); }

			std::vector<layer*>::const_iterator begin() const { return m_Layers.begin(); }
			std::vector<layer*>::const_iterator end()	const { return m_Layers.end(); }
			std::vector<layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
			std::vector<layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
		private:
			std::vector<layer*> m_Layers;
			unsigned int m_LayerInsertIndex = 0;
		};
	}
}