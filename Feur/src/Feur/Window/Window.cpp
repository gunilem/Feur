#include "fpch.h"
#include "Window.h"

#include <GLFW/glfw3.h>

namespace Feur {


	Window::Window(const WindowProperties& windowProperty)
		: m_Properties(windowProperty)
	{
		glfwInit();


		m_NativeWindow = glfwCreateWindow(m_Properties.Width, m_Properties.Height, m_Properties.Name, nullptr, nullptr);

		m_Root = UIElement::Create();
	}

	Window::~Window()
	{
		glfwDestroyWindow((GLFWwindow*)m_NativeWindow);
	}
}