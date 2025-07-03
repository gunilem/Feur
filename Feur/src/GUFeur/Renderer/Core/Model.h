#pragma once

#include "VertexBuffer.h"

#include "GUFeur/Core/RenderingAPI.h"

namespace GUFeur {
	struct Model {
		Buffer<Vertex>* VertexBuffer;
		Buffer<uint16_t>* IndexBuffer;

		Model(RenderingAPI& renderer, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices) {
			VertexBuffer = renderer.createVertexBuffer(vertices);
			IndexBuffer = renderer.createIndexBuffer(indices);
		}

		void cleanup(RenderingAPI& renderer) {
			renderer.cleanVertexBuffer(VertexBuffer);
			renderer.cleanIndexBuffer(IndexBuffer);
		}
	};
}