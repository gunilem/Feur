#include <Feur.h>

#include "SandboxApp.h"
#include "imgui/imgui.h"

using namespace Feur;

namespace Sandbox {

	//------ExampleLayer----------
	ExampleLayer::ExampleLayer()
		: m_Camera(OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f)), Layer("Example")
	{
		std::shared_ptr<VertexBuffer> vertexBuffer, squareVertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer, squareIndexBuffer;

		//------Triangle-------------
		//------VertexArray----------
		m_vertexArray.reset(VertexArray::Create());

		//------VertexBuffer---------
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0, 0.0, 1.0, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0, 1.0, 0.0, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0, 1.0, 1.0, 1.0f
		};

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout(layout);
		m_vertexArray->AddVertexbuffer(vertexBuffer);
		//---------------------------

		//------IndexBuffer----------
		uint32_t indices[3] = {
			0, 1 ,2
		};
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->SetIndexbuffer(indexBuffer);
		//---------------------------
		//---------------------------
		//---------------------------


		//------Square---------------
		//------VertexArray----------
		m_SquareVertexArray.reset(VertexArray::Create());


		//------VertexBuffer---------
		float squareVertices[4 * 7] = {
			-0.75f, -0.75f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
			 0.75f, -0.75f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
			 0.75f,  0.75f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f
		};

		squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->SetLayout(layout);
		m_SquareVertexArray->AddVertexbuffer(squareVertexBuffer);
		//---------------------------


		//------IndexBuffer----------
		uint32_t squareIndices[6] = {
			0, 1, 2,
			1, 3, 2
		};
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexbuffer(squareIndexBuffer);
		//---------------------------
		//---------------------------
		//---------------------------



		//------Shader---------------
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
				
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";
		std::string fragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
				
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(Shader::Create(vertexSrc, fragSrc));
		//---------------------------
	}


	void ExampleLayer::OnUpdate() {
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		MoveCamera();
		RotateCamera();

		Renderer::BeginScene(m_Camera);
		{
			Renderer::Submit(m_Shader, m_SquareVertexArray);

			Renderer::Submit(m_Shader, m_vertexArray);
		}
		Renderer::EndScene();
	}

	void ExampleLayer::OnImGuiRender() {}

	void ExampleLayer::OnEvent(Event& event) {}

	void ExampleLayer::MoveCamera() {
		if (Input::IsKeyPressed(F_KEY_D)) {
			m_CameraPosition.x += m_CameraMoveSpeed;
		}
		else if (Input::IsKeyPressed(F_KEY_A)) {
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}
		if (Input::IsKeyPressed(F_KEY_S)) {
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}
		else if (Input::IsKeyPressed(F_KEY_W)) {
			m_CameraPosition.y += m_CameraMoveSpeed;
		}
		if (Input::IsKeyPressed(F_KEY_Z)) {
			m_CameraPosition.z += m_CameraMoveSpeed;
		}
		else if (Input::IsKeyPressed(F_KEY_X)) {
			m_CameraPosition.z -= m_CameraMoveSpeed;
		}
		m_Camera.SetPosition(m_CameraPosition);
	}

	void ExampleLayer::RotateCamera() {
		if (Input::IsKeyPressed(F_KEY_R)) {
			m_CameraRotation += m_CameraRotationSpeed;
		}
		else if (Input::IsKeyPressed(F_KEY_T)) {
			m_CameraRotation -= m_CameraRotationSpeed;
		}
		m_Camera.SetRotation(m_CameraRotation);
	}
	//-------------------------


	
	//------Sandbox-------------
	Sandbox::Sandbox() {
		PushLayer(new ExampleLayer());
	}

	Sandbox::~Sandbox() {

	}
	//-------------------------
}

Application* Feur::CreateApplication() {
	return new Sandbox::Sandbox();
}