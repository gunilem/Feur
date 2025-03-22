#include "SandboxApp.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Feur;

namespace Sandbox {

	//------ExampleLayer----------
	ExampleLayer::ExampleLayer()
		: m_CameraController(1.6f / 0.9f, true), Layer("Example")
	{
		Ref<VertexBuffer> vertexBuffer, squareVertexBuffer;
		Ref<IndexBuffer> indexBuffer, squareIndexBuffer;

		m_DataWindow.VSync = Application::Get().GetWindow().IsVSync();

		//------Triangle-------------
		//------VertexArray----------
		m_vertexArray = VertexArray::Create();

		//------VertexBuffer---------
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		BufferLayout layoutTriangle = {
			{ ShaderDataType::Float3, "a_Position" }
		};

		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer->SetLayout(layoutTriangle);
		m_vertexArray->AddVertexbuffer(vertexBuffer);
		//---------------------------

		//------IndexBuffer----------
		uint32_t indices[3] = {
			0, 1 ,2
		};
		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_vertexArray->SetIndexbuffer(indexBuffer);
		//---------------------------
		//---------------------------
		//---------------------------


		//------Square---------------
		//------VertexArray----------
		m_SquareVertexArray = VertexArray::Create();


		//------VertexBuffer---------

		BufferLayout layoutSquare = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};

		float squareVertices[4 * 7] = {
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f,
			 0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f
		};

		squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVertexBuffer->SetLayout(layoutSquare);
		m_SquareVertexArray->AddVertexbuffer(squareVertexBuffer);
		//---------------------------


		//------IndexBuffer----------
		uint32_t squareIndices[6] = {
			0, 1, 2,
			1, 3, 2
		};
		squareIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVertexArray->SetIndexbuffer(squareIndexBuffer);
		//---------------------------
		//---------------------------
		//---------------------------


		//------Shader---------------
		auto textureShader = m_ShaderLibrary.Load("Texture", "assets/shaders/Texture.glsl");
		m_ShaderLibrary.Load("FlatColor", "assets/shaders/FlatColor.glsl");
		//---------------------------

		m_Texture = Texture2D::Create("assets/textures/france.png");

		textureShader->UploadUniformInt("u_MainTex", 0);
	}


	void ExampleLayer::OnUpdate() {
		// Update
		m_CameraController.OnUpdate();

		// Render
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		auto textureShader = m_ShaderLibrary.Get("Texture");
		auto colorShader = m_ShaderLibrary.Get("FlatColor");

		colorShader->UploadUniformFloat3("u_Color", m_SquareColor);
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
		PushLayer(new ExampleLayer());
	}

	Sandbox::~Sandbox() {

	}
	//-------------------------
}

Application* Feur::CreateApplication() {
	return new Sandbox::Sandbox();
}