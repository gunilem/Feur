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

		virtual void AddVertexbuffer(const std::shared_ptr<VertexBuffer>& buffer) override;
		virtual void SetIndexbuffer(const std::shared_ptr<IndexBuffer>& buffer) override;

		inline virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexbuffers() const override { return m_VertexBuffers; }
		inline virtual const std::shared_ptr<IndexBuffer> GetIndexbuffer() const override { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}

