#pragma once

#include "glm/glm.hpp"


namespace GUFeur {
	struct Vertex {
		glm::vec2 Position;
		glm::vec3 Color;
	};

	class VertexBuffer
	{

	public:
		VertexBuffer();
		VertexBuffer(std::vector<Vertex>& vertices);
		virtual ~VertexBuffer() {};

		/*
		VertexBuffer(VertexBuffer&& other);
		VertexBuffer(const VertexBuffer& other);
		VertexBuffer& operator=(VertexBuffer&& other);
		VertexBuffer& operator=(const VertexBuffer& other);
		*/
		void SetData(std::vector<Vertex>& vertices);
		size_t size() { return m_VertexCount; }

	protected:
		size_t m_VertexCount;
		std::vector<Vertex> m_Vertices;
	};
}

