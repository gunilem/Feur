#include "fpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Feur {
	Ref<Shader> Shader::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: F_CORE_ASSERT(false, "RendererAPI VertexBuffer: None Renderer API currently not supported!") return nullptr;
		case RendererAPI::API::OpenGL3: return std::make_shared<OpenGLShader>(path);
		}

		F_CORE_ASSERT(false, "RendererAPI: Unknown renderer API")
			return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: F_CORE_ASSERT(false, "RendererAPI VertexBuffer: None Renderer API currently not supported!") return nullptr;
		case RendererAPI::API::OpenGL3: return std::make_shared<OpenGLShader>(name, path);
		}

		F_CORE_ASSERT(false, "RendererAPI: Unknown renderer API")
			return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertSrc, const std::string& fragSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: F_CORE_ASSERT(false, "RendererAPI VertexBuffer: None Renderer API currently not supported!") return nullptr;
			case RendererAPI::API::OpenGL3: return std::make_shared<OpenGLShader>(name, vertSrc, fragSrc);
		}

		F_CORE_ASSERT(false, "RendererAPI: Unknown renderer API")
			return nullptr;
	}






	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();

		if (!Exists(name)) {
			m_Shaders[name] = shader;
			return;
		}
		F_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader Already Exist!");
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(name, filepath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		if (Exists(name))
			return m_Shaders[name];
		F_CORE_ASSERT(false, "Shader not find!");
		return nullptr;
	}
	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}