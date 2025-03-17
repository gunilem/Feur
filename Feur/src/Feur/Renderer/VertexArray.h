#pragma once

#include "Feur/Renderer/Buffer.h"

namespace Feur {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexbuffer(const std::shared_ptr<VertexBuffer>& buffer) = 0;
		virtual void SetIndexbuffer(const std::shared_ptr<IndexBuffer>& buffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexbuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer> GetIndexbuffer() const = 0;

		static VertexArray* Create();
	};

}