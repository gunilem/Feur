#include "fpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Feur {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}