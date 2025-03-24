#include "SandboxApp.h"
#include "Sandbox2D.h"

#include <Feur/Core/EntryPoint.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "imgui/imgui.h"

using namespace Feur;

namespace Sandbox {

	//------ExampleLayer----------
	ExampleLayer::ExampleLayer()
		: m_CameraController(1.6f / 0.9f, true), Layer("Example")
	{
		m_DataWindow.VSync = Application::Get().GetWindow().IsVSync();
	}


	void ExampleLayer::OnUpdate() {
		// Update
		m_CameraController.OnUpdate();

		// Render
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		auto textureShader = m_ShaderLibrary.Get("Texture");
		auto colorShader = m_ShaderLibrary.Get("FlatColor");

		//colorShader->UploadUniformFloat3("u_Color", m_SquareColor);
		Renderer::BeginScene(m_CameraController.GetCamera());
		{
			m_Texture->Bind(0);
			Renderer::Submit(textureShader, m_SquareVertexArray, m_SquareMat);
			Renderer::Submit(colorShader, m_vertexArray, m_SquareMat);

		}
		Renderer::EndScene();
	}

	void ExampleLayer::OnImGuiRender() 
	{
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

		ImGui::End();

		ImGui::Begin("Square");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

	void ExampleLayer::OnEvent(Event& e) 
	{
		m_CameraController.OnEvent(e);
	}
	//-------------------------


	
	//------Sandbox-------------
	Sandbox::Sandbox() {
		PushLayer(new Sandbox2D());
	}

	Sandbox::~Sandbox() {

	}
	//-------------------------
}

Application* Feur::CreateApplication() {
	return new Sandbox::Sandbox();
}