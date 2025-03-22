#include "fpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Feur {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: F_CORE_ASSERT(false, "RendererAPI VertexBuffer: None Renderer API currently not supported!") return nullptr;
		case RendererAPI::API::OpenGL3: return std::make_shared<OpenGLTexture2D>(path);
		}

		F_CORE_ASSERT(false, "RendererAPI: Unknown renderer API")
			return nullptr;
	}

}