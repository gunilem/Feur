#pragma once

#include <Feur.h>
#include <glm/glm.hpp>

namespace FeurEditor {
	class EditorLayer : public Feur::Layer {
	public:
		EditorLayer();


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Feur::Event& event) override;


	private:
		Feur::Ref<Feur::Texture2D> m_Texture;
		Feur::Ref<Feur::FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize;
		bool m_NeedRefresh = false;

		Feur::OrthographicCameraController m_CameraController;
		glm::vec4 m_color{0.8f, 0.2f, 0.3f, 1.0f};

	};

}
