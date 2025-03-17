#include "fpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Feur {

	VertexArray* VertexArray::Create() {

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: F_CORE_ASSERT(false, "RendererAPI VertexBuffer: None Renderer API currently not supported!") return nullptr;
			case RendererAPI::API::OpenGL3: return new OpenGLVertexArray();
		}

		F_CORE_ASSERT(false, "RendererAPI: Unknown renderer API")
			return nullptr;
	}
}