#pragma once

#include "Feur/Renderer/Buffer.h"

namespace Feur {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexbuffer(const Ref<VertexBuffer>& buffer) = 0;
		virtual void SetIndexbuffer(const Ref<IndexBuffer>& buffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexbuffers() const = 0;
		virtual const Ref<IndexBuffer> GetIndexbuffer() const = 0;

		static Ref<VertexArray> Create();
	};

}