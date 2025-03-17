#include "fpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Feur {
	Shader* Shader::Create(std::string& vertSrc, std::string& fragSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: F_CORE_ASSERT(false, "RendererAPI VertexBuffer: None Renderer API currently not supported!") return nullptr;
			case RendererAPI::API::OpenGL3: return new OpenGLShader(vertSrc, fragSrc);
		}

		F_CORE_ASSERT(false, "RendererAPI: Unknown renderer API")
			return nullptr;
	}
}