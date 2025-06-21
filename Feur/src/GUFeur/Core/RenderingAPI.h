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

		virtual Buffer<Vertex>* createVertexBuffer(std::vector<Vertex>& vertices) = 0;
		virtual Buffer<uint16_t>* createIndexBuffer(std::vector<uint16_t>& vertices) = 0;
		virtual void cleanVertexBuffer(Buffer<Vertex>* buffer) = 0;
		virtual void cleanIndexBuffer(Buffer<uint16_t>* buffer) = 0;
	};
}

