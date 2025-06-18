#include "fpch.h"

#include "Window.h"

namespace Feur {
	Window::Window(int width, int height, std::string name)
		: m_Width(width), m_Height(height), m_Name(name)
	{
		initWindow();
	}

	Window::~Window() {
		glfwDestroyWindow(m_NativeWindow);
		glfwTerminate();
	}

	void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, m_NativeWindow, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}

	void Window::frameBufferResizeCallback(GLFWwindow* nativeWindow, int width, int height)
	{
		auto window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(nativeWindow));
		window->m_FrameBufferResized = true;
		window->m_Width = width;
		window->m_Height = height;
	}

	void Window::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_NativeWindow = glfwCreateWindow(m_Width, m_Height, m_Name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_NativeWindow, this);
		glfwSetFramebufferSizeCallback(m_NativeWindow, frameBufferResizeCallback);
	}
}