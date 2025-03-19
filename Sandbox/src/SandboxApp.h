#pragma once


#include <Feur.h>

#include "imgui/imgui.h"

#include <glm/glm.hpp>

using namespace Feur;

namespace Sandbox {


	struct DataInfo {
		Timestep DeltaTime;
		float FPS;
		bool VSync;

		void SetData(Timestep DeltaTime, float FPS) {
			this->DeltaTime = DeltaTime;
			this->FPS = FPS;
		}
	};

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
		float m_CameraMoveSpeed{ 2.0f };

		float m_CameraRotation{ 0.0f };
		float m_CameraRotationSpeed{ 75.0f };

		DataInfo m_DataWindow;
		float m_lastDataRefresh;

		glm::mat4 m_SquareMat{ 1.0f };
	};


	class Sandbox : public Application {
	public:
		Sandbox();

		~Sandbox();
	};
}