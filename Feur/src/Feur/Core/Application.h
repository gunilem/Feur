#pragma once

#include "Core.h"

#include "Feur/Core/Timestep.h"

#include "Feur/Core/Window.h"
#include "Feur/Core/LayerStack.h"
#include "Feur/Events/Event.h"
#include "Feur/Events/ApplicationEvent.h"


#include "Feur/ImGui/ImGuiLayer.h"


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
		bool OnWindowResize(WindowResizeEvent e);

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_layerStack;

		Time m_Time;

	private:
		static Application* s_Instance;
	};

	// To be defined in the client
	Application* CreateApplication();
}