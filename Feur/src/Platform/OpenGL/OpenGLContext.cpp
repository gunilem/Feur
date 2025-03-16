#include "fpch.h"
#include "OpenGlContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Feur {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle{windowHandle} {
		F_CORE_ASSERT(m_windowHandle, "Window handle is null!");
	}


	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		F_CORE_ASSERT(status, "Failed to Initialize Glad!");

		F_CORE_INFO("OpenGL Vender : {0}", (char*)glGetString(GL_VENDOR));
		F_CORE_INFO("OpenGL Renderer : {0}", (char*)glGetString(GL_RENDERER));
		F_CORE_INFO("OpenGL Version : {0}", (char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() {



		glfwSwapBuffers(m_windowHandle);
	}

}