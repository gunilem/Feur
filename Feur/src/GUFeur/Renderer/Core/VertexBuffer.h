#pragma once

#include "glm/glm.hpp"


namespace GUFeur {
	enum class BufferTypes {
		None = 0,
		Vertex = 1,
		Index = 2
	};

	struct Vertex {
		glm::vec2 Position;
		glm::vec3 Color;
	};

	template<typename T>
	class Buffer {
	public:
		Buffer(BufferTypes type, std::vector<T>& data);
		virtual ~Buffer() {};

		/*
		VertexBuffer(VertexBuffer&& other);
		VertexBuffer(const VertexBuffer& other);
		VertexBuffer& operator=(VertexBuffer&& other);
		VertexBuffer& operator=(const VertexBuffer& other);
		*/
		void setData(std::vector<T>& data);

		size_t bufferDataCount() { return m_DataCount; }

		const std::vector<T>& bufferData() { return m_Data; }

		BufferTypes getType() { return m_BufferType; }

	protected:
		size_t m_DataCount;
		std::vector<T> m_Data;

		BufferTypes m_BufferType;
	};

	template<typename T>
	inline Buffer<T>::Buffer(BufferTypes type, std::vector<T>& data)
		: m_BufferType(type), m_Data{data}, m_DataCount{ data.size()}
	{
	}

	template<typename T>
	inline void Buffer<T>::setData(std::vector<T>& data)
	{
		m_Data = data;
		m_DataCount = m_Data.size();
	}

}

