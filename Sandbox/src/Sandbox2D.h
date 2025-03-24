#pragma once

#include <Feur.h>
#include <glm/glm.hpp>

namespace Sandbox {
	class Sandbox2D : public Feur::Layer {
	public:
		Sandbox2D();


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Feur::Event& event) override;


	private:
		Feur::Ref<Feur::Texture2D> m_Texture;

		Feur::OrthographicCameraController m_CameraController;
		glm::vec4 m_color{0.8f, 0.2f, 0.3f, 1.0f};

	};

}
