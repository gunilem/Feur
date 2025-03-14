#pragma once

#include "Core.h"

#include "Window.h"
#include "Feur/LayerStack.h"
#include "Feur/Events/Event.h"
#include "Feur/Events/ApplicationEvent.h"


namespace Feur {

	class FEUR_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_layerStack;

	private:
		static Application* s_Instance;
	};

	// To be defined in the client
	Application* CreateApplication();
}