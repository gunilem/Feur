#pragma once

#include <string>
#include "Feur/Core/Core.h"

namespace Feur {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void SetData(const void* data) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
	};
}
