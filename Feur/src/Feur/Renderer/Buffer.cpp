#include "fpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Feur {





	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_Elements(elements) {

		CalculateOffsetAndStride();
	}
	void BufferLayout::CalculateOffsetAndStride() {
		uint32_t offset = 0;
		m_Stride = 0;

		for (auto& element : m_Elements) {
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}


	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: F_CORE_ASSERT(false, "RendererAPI VertexBuffer: None Renderer API currently not supported!") return nullptr;
			case RendererAPI::API::OpenGL3: return std::make_shared<OpenGLVertexBuffer>(size);
		}

		F_CORE_ASSERT(false, "RendererAPI: Unknown renderer API")
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: F_CORE_ASSERT(false, "RendererAPI VertexBuffer: None Renderer API currently not supported!") return nullptr;
			case RendererAPI::API::OpenGL3: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		F_CORE_ASSERT(false, "RendererAPI: Unknown renderer API")
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: F_CORE_ASSERT(false, "RendererAPI IndexBuffer: None Renderer API currently not supported!") return nullptr;
		case RendererAPI::API::OpenGL3: return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		F_CORE_ASSERT(false, "RendererAPI: Unknown renderer API")
			return nullptr;
	}
}