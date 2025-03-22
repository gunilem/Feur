#include "fpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace Feur {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") return GL_VERTEX_SHADER;
		else if (type == "fragment") return GL_FRAGMENT_SHADER;

		F_CORE_ASSERT(false, "Unknown sahder type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string source = ReadFile(path);
		auto shaderSources = PreProcess(source);

		Compile(shaderSources);

		//Extract name from filepath
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;

		auto lastDot = path.rfind(".");
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		m_Name = path.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& path)
		: m_Name(name)
	{
		std::string source = ReadFile(path);
		auto shaderSources = PreProcess(source);

		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragsrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragsrc;

		Compile(sources);
	}


	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			F_CORE_ASSERT(in, "Could not open file '{0}'", path);
		}


		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map < GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			F_CORE_ASSERT(eol != std::string::npos, "Syntax Error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			F_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specifier");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		F_CORE_ASSERT(location != -1, "OpenGLShader: uniform name does not exist!")

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		F_CORE_ASSERT(location != -1, "OpenGLShader: uniform name does not exist!")

		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		F_CORE_ASSERT(location != -1, "OpenGLShader: uniform name does not exist!")

		glUniform1i(location, value);
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();

		F_CORE_ASSERT(shaderSources.size() <= 2, "There sould be less than 2 shaders")
		std::array<GLenum, 2> glShaderIDs;
		int glshaderIdIndex = 0;

		for (auto& kv : shaderSources) {

			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* GLsource = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &GLsource, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				F_CORE_ERROR("{0}", infoLog.data());
				F_CORE_ASSERT(false, "Shader compilation failure!");

				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glshaderIdIndex++] = shader;
		}
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			for (auto id : glShaderIDs) {
				glDeleteShader(id);
			}

			F_CORE_ERROR("{0}", infoLog.data());
			F_CORE_ASSERT(false, "Shader linking failure!");

			return;
		}

		for (auto id : glShaderIDs) {
			glDetachShader(program, id);
		}

		m_RendererID = program;
	}
}