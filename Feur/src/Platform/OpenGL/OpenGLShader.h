#pragma once

#include "Feur/Renderer/Shader.h"

namespace Feur {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(std::string& vertexSrc, std::string& fragsrc);
		virtual ~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;

	private:
		uint32_t m_RendererID;
	};
}