#pragma once

#include "Layer.h"

namespace Feur {

	class LayerStack
	{
	public:
		LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void PopLayer();
		void PopOverlay();

		std::vector<Layer*>::iterator begin() {
			return m_Layers.begin();
		}

		std::vector<Layer*>::iterator end() {
			return m_Layers.end();
		}

		std::vector<Layer*>::const_iterator begin() const {
			return m_Layers.begin();
		}

		std::vector<Layer*>::const_iterator end() const {
			return m_Layers.end();
		}

	private:
		std::vector<Layer*> m_Layers;
		int m_OverlayFirstIndex;
	};

}
