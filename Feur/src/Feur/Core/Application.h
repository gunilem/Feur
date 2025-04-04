#pragma once

#include "Feur/Core/LayerStack.h"
#include "Feur/Window/Window.h"

namespace Feur {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		virtual void Run();

		virtual void PushLayer(Layer* layer);
		virtual void PushOverlay(Layer* overlay);

	private:
		LayerStack m_LayerStack;
		Window* m_Window;
	};

}
