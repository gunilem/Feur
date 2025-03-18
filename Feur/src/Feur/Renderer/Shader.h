#pragma once

#include <glm/glm.hpp>

namespace Feur {

	class Shader
	{
	public:
		virtual ~Shader() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;

		static Shader* Create(std::string& vertSrc, std::string& fragSrc);
	};

}
