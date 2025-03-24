#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>

namespace Sandbox {

#define PROFILE_SCOPE(name) Feur::Timer timer##__LINE__(name, [&](ProfileResult result) {m_ProfileResults.push_back(result);})

	Sandbox2D::Sandbox2D()
		: m_CameraController(1.6f / 0.9f), Layer("Sandbox2D")
	{
		Feur::Application::Get().GetWindow().SetVSync(true);
		m_Texture = Feur::Texture2D::Create("assets/textures/Botanicat.png");
	}


	void Sandbox2D::OnAttach()
	{
	}

	void Sandbox2D::OnDetach()
	{
	}

	void Sandbox2D::OnUpdate() {
		F_PROFILE_FUNCTION();

		// Update
		m_CameraController.OnUpdate();
		

		// Render
		{
			F_PROFILE_SCOPE("Renderer Setup");
			Feur::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			Feur::RenderCommand::Clear();
		}

		{
			F_PROFILE_SCOPE("Renderer Draw");
			Feur::Renderer2D::BeginScene(m_CameraController.GetCamera());
			{
				Feur::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_color);
				Feur::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 4.0f, 4.0f }, m_Texture);
			}
			Feur::Renderer2D::EndScene();
		}
	}

	void Sandbox2D::OnImGuiRender()
	{
		F_PROFILE_FUNCTION();
		/*
		ImGui::Begin("Data");

		if (Time::GetTime() - m_lastDataRefresh >= 1.0f) {
			m_DataWindow.SetData(Time::GetDeltaTime(), 1 / Time::GetDeltaTime());
			m_lastDataRefresh = Time::GetTime();
		}

		std::ostringstream os;
		os << "Delta Time : " << m_DataWindow.DeltaTime << " ( " << m_DataWindow.DeltaTime.GetMilliseconds() << " millisecondes)";
		ImGui::Text(os.str().c_str());
		std::ostringstream os2;
		os2 << "FrameRate : " << m_DataWindow.FPS << " FPS";
		ImGui::Text(os2.str().c_str());
		if (ImGui::Checkbox("VSync", &m_DataWindow.VSync)) {
			Application::Get().GetWindow().SetVSync(m_DataWindow.VSync);
		}
		*/

		ImGui::Begin("Square");

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_color));

		ImGui::End();


	}

	void Sandbox2D::OnEvent(Feur::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}