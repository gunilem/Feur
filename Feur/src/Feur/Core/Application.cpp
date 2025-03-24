#include "fpch.h"
#include "Application.h"

#include "Feur/Renderer/Renderer.h"
#include "Feur/Renderer/Renderer2D.h"

namespace Feur {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		F_PROFILE_FUNCTION();

		F_CORE_ASSERT(!s_Instance, "There should be one Application!")
		s_Instance = this;

		m_Window = Window::Create();
		
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_Time = Time();
	}

	Application::~Application() {
		F_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}


	void Application::PushLayer(Layer* layer) {
		F_PROFILE_FUNCTION();

		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		F_PROFILE_FUNCTION();

		m_layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}


	void Application::OnEvent(Event& e) {
		F_PROFILE_FUNCTION();

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
		F_PROFILE_FUNCTION();

		while (m_Running) {
			F_PROFILE_SCOPE("Run Loop");

			m_Time.UpdateDeltaTime();

			if (!m_Minimized) {
				{
					F_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_layerStack) {
						layer->OnUpdate();
					}
				}
			}

			{
				F_PROFILE_SCOPE("LayerStack OnImGuiRender");

				m_ImGuiLayer->Begin();
				for (Layer* layer : m_layerStack) {
					layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent e) {
		F_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
};