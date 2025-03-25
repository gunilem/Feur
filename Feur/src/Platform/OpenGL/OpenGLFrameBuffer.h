#pragma once

#include "Feur/Renderer/FrameBuffer.h"

namespace Feur {
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specs);
		virtual ~OpenGLFrameBuffer();


		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		void Invalidate();


		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment, m_DepthAttachment = 0;
		FrameBufferSpecification m_Specification;
	};
}

