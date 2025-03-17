#pragma once

#include "Feur/Renderer/Shader.h"

namespace Feur {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(std::string& vertexSrc, std::string& fragsrc);
		virtual ~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

	private:
		uint32_t m_RendererID;
	};
}