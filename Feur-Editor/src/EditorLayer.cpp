#include "EditorLayer.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>


namespace FeurEditor {

#define PROFILE_SCOPE(name) Feur::Timer timer##__LINE__(name, [&](ProfileResult result) {m_ProfileResults.push_back(result);})

    EditorLayer::EditorLayer()
		: m_CameraController(1.6f / 0.9f), Layer("Sandbox2D")
	{
		Feur::Application::Get().GetWindow().SetVSync(true);
		m_Texture = Feur::Texture2D::Create("assets/textures/Botanicat.png");
	}


	void EditorLayer::OnAttach()
	{
        Feur::FrameBufferSpecification specs;
        specs.Width = 1280;
        specs.Height = 720;

        m_FrameBuffer = Feur::FrameBuffer::Create(specs);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate() {
		F_PROFILE_FUNCTION();

		// Update
		m_CameraController.OnUpdate();
		
        if (m_NeedRefresh) {
            m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
        }

		// Render
		{
            m_FrameBuffer->Bind();
			F_PROFILE_SCOPE("Renderer Setup");
			Feur::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			Feur::RenderCommand::Clear();
		}

		{
			F_PROFILE_SCOPE("Renderer Draw");
			Feur::Renderer2D::BeginScene(m_CameraController.GetCamera());
			{
				Feur::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f, m_color);
				Feur::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 4.0f, 4.0f }, 0.0f, m_Texture);
			}
			Feur::Renderer2D::EndScene();
            m_FrameBuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        bool* p_open = new bool(true);
        ImGui::Begin("DockSpace Demo", p_open, window_flags);
        ImGui::PopStyleVar();

        ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                    *p_open = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Scene Viewport");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize)) {
            m_NeedRefresh = true;
            m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        }
        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((ImTextureID)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();


	}

	void EditorLayer::OnEvent(Feur::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}