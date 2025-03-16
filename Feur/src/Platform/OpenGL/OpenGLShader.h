#pragma once

#include <string>

namespace Feur {

	class OpenGLShader {
	public:
		OpenGLShader(std::string& vertexSrc, std::string& fragsrc);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_RendererID;
	};
}