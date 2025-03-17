#pragma once

#include "RenderCommand.h"

namespace Feur {

	class Renderer {
	public:
		static void BeginScene(); //TODO : add scene params
		static void EndScene(); 

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}