#include "fpch.h"
#include "Renderer2D.h"

#include "Feur/Renderer/VertexArray.h"
#include "Feur/Renderer/Shader.h"
#include "Feur/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Feur {

	
	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexId;
		float TillingFactor;
	};

	struct Renderer2DData {
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlot = 32;

		Ref<VertexArray> VertexArrayData;
		Ref<VertexBuffer> VertexBufferData;
		Ref<Shader> FlatColorShaderData;
		Ref<Shader> TextureShaderData;

		uint32_t IndexCount = 0;
		QuadVertex* VertexBufferBase = nullptr;
		QuadVertex* VertexBufferPtr = nullptr;

		Ref<Texture2D> WhiteTexture;
		std::array<Ref<Texture2D>, MaxTextureSlot> TextureSlots;
		float SlotIndex = 1;

		glm::vec4 vertexPosition[4];
	};

	static Renderer2DData s_QuadData;
	static BatchData s_BatchData;




	void Renderer2D::Init()
	{
		F_PROFILE_FUNCTION();

		s_QuadData.VertexArrayData = VertexArray::Create();

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TextId" },
			{ ShaderDataType::Float, "a_tillingFactor" }
		};

		s_QuadData.VertexBufferData = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(QuadVertex));

		s_QuadData.VertexBufferData->SetLayout(layout);

		s_QuadData.VertexBufferBase = new QuadVertex[Renderer2DData::MaxVertices];
		s_QuadData.VertexArrayData->AddVertexbuffer(s_QuadData.VertexBufferData);

		uint32_t* indices = new uint32_t[Renderer2DData::MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < Renderer2DData::MaxIndices; i += 6) {
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, Renderer2DData::MaxIndices);
		s_QuadData.VertexArrayData->SetIndexbuffer(indexBuffer);
		delete[] indices;


		/*
		s_QuadData.FlatColorShaderData = Shader::Create("assets/shaders/FlatColor.glsl");*/
		s_QuadData.TextureShaderData = Shader::Create("assets/shaders/Texture.glsl");
		s_QuadData.TextureShaderData->Bind();

		int32_t samplers[Renderer2DData::MaxTextureSlot];
		for (int32_t i = 0; i < Renderer2DData::MaxTextureSlot; i++) {
			samplers[i] = i;
		}
		s_QuadData.TextureShaderData->SetIntArray("u_Textures", samplers, 32);

		s_QuadData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexture = 0xffffffff;
		s_QuadData.WhiteTexture->SetData(&whiteTexture);

		s_QuadData.TextureSlots[0] = s_QuadData.WhiteTexture;

		s_QuadData.vertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_QuadData.vertexPosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_QuadData.vertexPosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_QuadData.vertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		F_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_BatchData.Reset();
		F_PROFILE_FUNCTION();

		//s_QuadData.FlatColorShaderData->Bind();
		//s_QuadData.FlatColorShaderData->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_QuadData.TextureShaderData->Bind();
		s_QuadData.TextureShaderData->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		ResetBatch();
	}

	void Renderer2D::EndScene()
	{
		F_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		if (s_QuadData.IndexCount >= Renderer2DData::MaxIndices) {
			Flush();
		}

		float texID = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		s_QuadData.VertexBufferPtr->Position = transform * s_QuadData.vertexPosition[0];
		s_QuadData.VertexBufferPtr->Color = color;
		s_QuadData.VertexBufferPtr->TexCoord = {0.0f, 0.0f};
		s_QuadData.VertexBufferPtr->TexId = texID;
		s_QuadData.VertexBufferPtr->TillingFactor = 1.0f;
		s_QuadData.VertexBufferPtr++;

		s_QuadData.VertexBufferPtr->Position = transform * s_QuadData.vertexPosition[1];
		s_QuadData.VertexBufferPtr->Color = color;
		s_QuadData.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_QuadData.VertexBufferPtr->TexId = texID;
		s_QuadData.VertexBufferPtr->TillingFactor = 1.0f;
		s_QuadData.VertexBufferPtr++;

		s_QuadData.VertexBufferPtr->Position = transform * s_QuadData.vertexPosition[2];
		s_QuadData.VertexBufferPtr->Color = color;
		s_QuadData.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_QuadData.VertexBufferPtr->TexId = texID;
		s_QuadData.VertexBufferPtr->TillingFactor = 1.0f;
		s_QuadData.VertexBufferPtr++;

		s_QuadData.VertexBufferPtr->Position = transform * s_QuadData.vertexPosition[3];
		s_QuadData.VertexBufferPtr->Color = color;
		s_QuadData.VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_QuadData.VertexBufferPtr->TexId = texID;
		s_QuadData.VertexBufferPtr->TillingFactor = 1.0f;
		s_QuadData.VertexBufferPtr++;

		s_QuadData.IndexCount += 6;
		s_BatchData.Quads++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const float tillingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tillingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const float tillingFactor)
	{
		if (s_QuadData.SlotIndex >= Renderer2DData::MaxTextureSlot) {
			Flush();
		}
		if (s_QuadData.IndexCount >= Renderer2DData::MaxIndices) {
			Flush();
		}

		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float currentIndex = 0.0f;
		for (int i = 0; i < s_QuadData.SlotIndex; i++)
		{
			if (*s_QuadData.TextureSlots[i].get() == *texture.get()) {
				currentIndex = (float)i;
				break;
			}
		}

		if (currentIndex == 0.0f) {
			currentIndex = s_QuadData.SlotIndex++;
			s_QuadData.TextureSlots[(int)currentIndex] = texture;
		}


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_QuadData.VertexBufferPtr->Position = transform * s_QuadData.vertexPosition[0];
		s_QuadData.VertexBufferPtr->Color = color;
		s_QuadData.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_QuadData.VertexBufferPtr->TexId = currentIndex;
		s_QuadData.VertexBufferPtr->TillingFactor = tillingFactor;
		s_QuadData.VertexBufferPtr++;

		s_QuadData.VertexBufferPtr->Position = transform * s_QuadData.vertexPosition[1];
		s_QuadData.VertexBufferPtr->Color = color;
		s_QuadData.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_QuadData.VertexBufferPtr->TexId = currentIndex;
		s_QuadData.VertexBufferPtr->TillingFactor = tillingFactor;
		s_QuadData.VertexBufferPtr++;

		s_QuadData.VertexBufferPtr->Position = transform * s_QuadData.vertexPosition[2];
		s_QuadData.VertexBufferPtr->Color = color;
		s_QuadData.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_QuadData.VertexBufferPtr->TexId = currentIndex;
		s_QuadData.VertexBufferPtr->TillingFactor = tillingFactor;
		s_QuadData.VertexBufferPtr++;

		s_QuadData.VertexBufferPtr->Position = transform * s_QuadData.vertexPosition[3];
		s_QuadData.VertexBufferPtr->Color = color;
		s_QuadData.VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_QuadData.VertexBufferPtr->TexId = currentIndex;
		s_QuadData.VertexBufferPtr->TillingFactor = tillingFactor;
		s_QuadData.VertexBufferPtr++;

		s_QuadData.IndexCount += 6;

		s_BatchData.Quads++;
	}

	const BatchData& Renderer2D::GetBatchData()
	{
		return s_BatchData;
	}

	void Renderer2D::Flush()
	{
		for (int i = 0; i < s_QuadData.SlotIndex; i++) {
			s_QuadData.TextureSlots[i]->Bind(i);
		}
		uint32_t dataSize = (uint8_t*)s_QuadData.VertexBufferPtr - (uint8_t*)s_QuadData.VertexBufferBase;
		s_QuadData.VertexBufferData->SetData(s_QuadData.VertexBufferBase, dataSize);

		RenderCommand::DrawIndexed(s_QuadData.VertexArrayData, s_QuadData.IndexCount);
		s_BatchData.Batch++;

		ResetBatch();
	}

	void Renderer2D::ResetBatch()
	{
		s_QuadData.IndexCount = 0;
		s_QuadData.VertexBufferPtr = s_QuadData.VertexBufferBase;

		s_QuadData.SlotIndex = 1.0f;
	}



	/*
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		F_PROFILE_FUNCTION();

		s_QuadData.FlatColorShaderData->Bind();
		s_QuadData.FlatColorShaderData->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		s_QuadData.FlatColorShaderData->SetMat4("u_Transform", transform);

		s_QuadData.VertexArrayData->Bind();
		RenderCommand::DrawIndexed(s_QuadData.VertexArrayData);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		F_PROFILE_FUNCTION();

		s_QuadData.TextureShaderData->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

		s_QuadData.TextureShaderData->SetMat4("u_Transform", transform);

		texture->Bind();

		s_QuadData.VertexArrayData->Bind();
		RenderCommand::DrawIndexed(s_QuadData.VertexArrayData);
	}*/
}