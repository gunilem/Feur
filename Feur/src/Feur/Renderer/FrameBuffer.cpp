#include "fpch.h"
#include "FrameBuffer.h"

#include "Feur/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Feur {
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specs)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: F_CORE_ASSERT(false, "RendererAPI VertexBuffer: None Renderer API currently not supported!") return nullptr;
		case RendererAPI::API::OpenGL3: return std::make_shared<OpenGLFrameBuffer>(specs);
		}

		F_CORE_ASSERT(false, "RendererAPI: Unknown renderer API")
			return nullptr;
	}

}