#pragma once

#include "GUFeur/Core/Core.h"

#include "GUFeur/Renderer/Core/VertexBuffer.h"

struct GLFWwindow;

namespace GUFeur {
	class RenderingAPI
	{
	public:
		static RenderingAPI* create();

	public:
		RenderingAPI() {};
		virtual ~RenderingAPI() {};

		GUF_DELETE_COPY_CONSTRUCTORS(RenderingAPI)

			virtual void init(uint32_t windowWidth, uint32_t windowHeight) = 0;
		virtual void cleanup() = 0;

		virtual void drawFrame() = 0;

		virtual void OnWindowResized(uint32_t windowWidth, uint32_t windowHeight) = 0;

		virtual VertexBuffer* createVertexBuffer(std::vector<Vertex>& vertices) = 0;
		virtual void cleanVertexBuffer(VertexBuffer* buffer) = 0;
	};
}

