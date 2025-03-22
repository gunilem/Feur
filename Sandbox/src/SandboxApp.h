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

		void OnEvent(Event& event) override;

	private:
		ShaderLibrary m_ShaderLibrary;
		Ref<VertexArray> m_vertexArray;
		Ref<VertexArray> m_SquareVertexArray;
		Ref<Texture2D> m_Texture;

		OrthographicCameraController m_CameraController;

		DataInfo m_DataWindow;
		float m_lastDataRefresh;

		glm::mat4 m_SquareMat{ 1.0f };
		glm::vec3 m_SquareColor{ 1.0f, 0.0f, 0.0f};

		
	};


	class Sandbox : public Application {
	public:
		Sandbox();

		~Sandbox();
	};
}