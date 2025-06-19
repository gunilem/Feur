#include "fpch.h"
#include "VertexBuffer.h"

namespace GUFeur {
	VertexBuffer::VertexBuffer()
		: m_Vertices{}, m_VertexCount{0}
	{

	}

	VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices)
		: m_Vertices{ vertices }, m_VertexCount{ vertices.size()}
	{

	}
	/*
	VertexBuffer::VertexBuffer(VertexBuffer&& other)
	{
		other.m_VertexBuffer = m_VertexBuffer;
		other.m_VertexCount = m_VertexCount;
	}

	VertexBuffer::VertexBuffer(const VertexBuffer& other)
	{
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
	{
		// TODO: insert return statement here
	}

	VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other)
	{
		// TODO: insert return statement here
	}*/

	void VertexBuffer::SetData(std::vector<Vertex>& vertices)
	{
		m_Vertices = vertices;
		m_VertexCount = m_Vertices.size();
	}

}