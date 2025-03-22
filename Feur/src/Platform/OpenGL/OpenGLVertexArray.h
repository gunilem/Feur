#pragma once

#include "Feur/Renderer/VertexArray.h"

namespace Feur {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexbuffer(const Ref<VertexBuffer>& buffer) override;
		virtual void SetIndexbuffer(const Ref<IndexBuffer>& buffer) override;

		inline virtual const std::vector<Ref<VertexBuffer>>& GetVertexbuffers() const override { return m_VertexBuffers; }
		inline virtual const Ref<IndexBuffer> GetIndexbuffer() const override { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}

