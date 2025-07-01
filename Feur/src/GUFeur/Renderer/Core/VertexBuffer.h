#pragma once

#include "glm/glm.hpp"


namespace GUFeur {
	enum class BufferTypes {
		None = 0,
		Vertex = 1,
		Index = 2,
		Uniform = 3
	};

	struct Vertex {
		glm::vec2 Position;
		glm::vec3 Color;
	};

	template<typename T>
	class Buffer {
	public:
		Buffer(BufferTypes type, T* dataPtr, size_t count);
		virtual ~Buffer() {};

		/*
		VertexBuffer(VertexBuffer&& other);
		VertexBuffer(const VertexBuffer& other);
		VertexBuffer& operator=(VertexBuffer&& other);
		VertexBuffer& operator=(const VertexBuffer& other);
		*/

		size_t bufferDataCount() { return m_DataCount; }

		T* bufferData() { return m_DataPtr; }

		BufferTypes getType() { return m_BufferType; }

	protected:
		size_t m_DataCount;
		T* m_DataPtr;

		BufferTypes m_BufferType;
	};

	template<typename T>
	inline Buffer<T>::Buffer(BufferTypes type, T* dataPtr, size_t count)
		: m_BufferType(type), m_DataPtr{ dataPtr }, m_DataCount{ count }
	{
	}

}

