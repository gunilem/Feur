#pragma once

#include <glad/glad.h>
#include "Feur/Renderer/Shader.h"

namespace Feur {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& path);
		OpenGLShader(const std::string& name, const std::string& path);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragsrc);
		virtual ~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; };

		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& values) override;
		virtual void UploadUniformInt(const std::string& name, const int value) override;

	private:
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}