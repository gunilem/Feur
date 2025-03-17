#include <Feur.h>

#include "imgui/imgui.h"

using namespace Feur;

class ExampleLayer : public Feur::Layer {
public:
	ExampleLayer()
		: Layer("Example") 
	{ 
		std::shared_ptr<VertexBuffer> vertexBuffer, squareVertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer, squareIndexBuffer;

		//Creation of the Vertex array
		m_vertexArray.reset(VertexArray::Create());

		//Creation of vertex buffer
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


		//Creation of index buffer
		uint32_t indices[3] = {
			0, 1 ,2
		};
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->SetIndexbuffer(indexBuffer);



		//Creation of the Vertex array
		m_SquareVertexArray.reset(VertexArray::Create());


		//Creation of vertex buffer
		float squareVertices[4 * 7] = {
			-0.75f, -0.75f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
			 0.75f, -0.75f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
			 0.75f,  0.75f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f
		};

		squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->SetLayout(layout);
		m_SquareVertexArray->AddVertexbuffer(squareVertexBuffer);


		//Creation of index buffer
		uint32_t squareIndices[6] = {
			0, 1, 2,
			1, 3, 2
		};
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexbuffer(squareIndexBuffer);


		//creation shader
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
				
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
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
	
	}

	void OnUpdate() override {
		//F_INFO("ExampleLayer::Update");

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();


		Renderer::BeginScene();
		{
			m_Shader->Bind();
			Renderer::Submit(m_SquareVertexArray);

			Renderer::Submit(m_vertexArray);
		}
		Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();

	}

	void OnEvent(Event& event) override {
		//F_TRACE("{0}", event.ToString());
	}

	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_vertexArray;

	std::shared_ptr<VertexArray> m_SquareVertexArray;
};

class Sandbox : public Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Application* Feur::CreateApplication() {
	return new Sandbox();
}