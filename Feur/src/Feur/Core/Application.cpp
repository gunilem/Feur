#include "fpch.h"
#include "Application.h"

#include "Feur/Renderer/Renderer.h"

namespace Feur {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		F_CORE_ASSERT(!s_Instance, "There should be one Application!")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_Time = Time();
	}

	Application::~Application() {

	}


	void Application::PushLayer(Layer* layer) {
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}


	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) {
				break;
			}
		}
	}


	void Application::Run() {

		while (m_Running) {

			m_Time.UpdateDeltaTime();

			if (!m_Minimized) {
				for (Layer* layer : m_layerStack) {
					layer->OnUpdate();
				}
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_layerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent e) {
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
};