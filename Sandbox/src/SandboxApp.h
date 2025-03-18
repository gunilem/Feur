#pragma once


#include <Feur.h>

#include "imgui/imgui.h"

using namespace Feur;

namespace Sandbox {

	class ExampleLayer : public Feur::Layer {
	public:
		ExampleLayer();

		void OnUpdate() override;

		virtual void OnImGuiRender() override;

		void MoveCamera();
		void RotateCamera();

		void OnEvent(Event& event) override;

	private:
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_vertexArray;

		std::shared_ptr<VertexArray> m_SquareVertexArray;
		Camera m_Camera;

		glm::vec3 m_CameraPosition{0.0f};
		float m_CameraMoveSpeed{ 0.01f };

		float m_CameraRotation{ 0.0f };
		float m_CameraRotationSpeed{ 1.0f };
	};


	class Sandbox : public Application {
	public:
		Sandbox();

		~Sandbox();
	};
}