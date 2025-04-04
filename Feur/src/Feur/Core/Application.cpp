#include "fpch.h"
#include "Application.h"



namespace Feur {
	Application::Application()
	{
		m_LayerStack = LayerStack();

		WindowProperties props;
		props.Name = "Editor";
		props.X = 10; 
		props.Y = 10;
		props.Width = 1080;
		props.Height = 720;

		m_Window = new Window(props);
	}

	Application::~Application()
	{
		delete m_Window;
	}

	void Application::Run()
	{
		while (true) {
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}
}