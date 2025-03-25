#pragma once

#include "Feur/Renderer/Buffer.h"

namespace Feur {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline virtual const BufferLayout& GetLayout() const { return m_Layout; };
		inline virtual void SetLayout(BufferLayout& layout) override { m_Layout = layout; };
		virtual void SetData(const void* data, uint32_t size);


	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		virtual uint32_t GetCount() const override { return m_Count; };

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}

