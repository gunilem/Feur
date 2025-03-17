#pragma once

namespace Feur {

	class Shader
	{
	public:
		virtual ~Shader() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(std::string& vertSrc, std::string& fragSrc);
	};

}
