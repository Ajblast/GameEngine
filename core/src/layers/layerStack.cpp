#include "gravpch.h"
#include "layerStack.h"

GRAVEngine::Layers::layerStack::~layerStack()
{
	for (layer* layer : m_Layers)
	{
		// TODO: Memory management of layers
		layer->onDetach();
		delete layer;
	}
}

void GRAVEngine::Layers::layerStack::pushLayer(layer* layer)
{
	m_Layers.emplace(begin() + m_LayerInsertIndex, layer);
	m_LayerInsertIndex++;
}

void GRAVEngine::Layers::layerStack::pushOverlay(layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

void GRAVEngine::Layers::layerStack::popLayer(layer* layer)
{
	auto it = std::find(begin(), end() + m_LayerInsertIndex, layer);
	if (it != m_Layers.begin() + m_LayerInsertIndex)
	{
		layer->onDetach();
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}
}

void GRAVEngine::Layers::layerStack::popOverlay(layer* overlay)
{
	auto it = std::find(begin() + m_LayerInsertIndex, end(), overlay);
	if (it != m_Layers.end())
	{
		overlay->onDetach();
		m_Layers.erase(it);
	}
}
