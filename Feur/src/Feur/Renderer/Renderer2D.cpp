#include "fpch.h"
#include "Renderer2D.h"

#include "Feur/Renderer/VertexArray.h"
#include "Feur/Renderer/Shader.h"
#include "Feur/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Feur {

	struct Renderer2DStorage {
		Ref<VertexArray> VertexArrayData;
		Ref<Shader> FlatColorShaderData;
		Ref<Shader> TextureShaderData;
	};

	static Renderer2DStorage* s_QuadData;

	void Renderer2D::Init()
	{
		F_PROFILE_FUNCTION();

		s_QuadData = new Renderer2DStorage();
		s_QuadData->VertexArrayData = VertexArray::Create();

		BufferLayout squareLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};

		float squareVertices[4 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
		};
		Ref<VertexBuffer> squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		squareVertexBuffer->SetLayout(squareLayout);

		uint32_t squareIndices[6] = {
			0, 1, 2,
			1, 3, 2
		};
		Ref<IndexBuffer> squareIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices));

		s_QuadData->VertexArrayData->AddVertexbuffer(squareVertexBuffer);
		s_QuadData->VertexArrayData->SetIndexbuffer(squareIndexBuffer);

		s_QuadData->FlatColorShaderData = Shader::Create("assets/shaders/FlatColor.glsl");
		s_QuadData->TextureShaderData = Shader::Create("assets/shaders/Texture.glsl");

		s_QuadData->TextureShaderData->Bind();
		s_QuadData->TextureShaderData->SetInt("u_MainTex", 0);

	}

	void Renderer2D::Shutdown()
	{
		F_PROFILE_FUNCTION();

		delete s_QuadData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		F_PROFILE_FUNCTION();

		s_QuadData->FlatColorShaderData->Bind();
		s_QuadData->FlatColorShaderData->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_QuadData->TextureShaderData->Bind();
		s_QuadData->TextureShaderData->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		F_PROFILE_FUNCTION();

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		F_PROFILE_FUNCTION();

		s_QuadData->FlatColorShaderData->Bind();
		s_QuadData->FlatColorShaderData->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		s_QuadData->FlatColorShaderData->SetMat4("u_Transform", transform);

		s_QuadData->VertexArrayData->Bind();
		RenderCommand::DrawIndexed(s_QuadData->VertexArrayData);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		F_PROFILE_FUNCTION();

		s_QuadData->TextureShaderData->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		s_QuadData->TextureShaderData->SetMat4("u_Transform", transform);

		texture->Bind();

		s_QuadData->VertexArrayData->Bind();
		RenderCommand::DrawIndexed(s_QuadData->VertexArrayData);
	}
}