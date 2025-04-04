#include "fpch.h"
#include "LayerStack.h"


namespace Feur {
	LayerStack::LayerStack()
	{
		m_Layers = std::vector<Layer*>();
		m_OverlayFirstIndex = 0;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_OverlayFirstIndex++, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer()
	{
		m_Layers.erase(m_Layers.begin() + m_OverlayFirstIndex--);
	}

	void LayerStack::PopOverlay()
	{
		m_Layers.erase(m_Layers.end() - 1);
	}
}