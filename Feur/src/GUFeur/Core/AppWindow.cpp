#include "fpch.h"
#include "AppWindow.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace GUFeur {


	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
		app->OnWindowResized(width, height);
	}

	AppWindow::AppWindow(uint32_t width, uint32_t height, const char* name)
	{
		m_windowProperties = AppWindowProperties();
		m_windowProperties.Width = width;
		m_windowProperties.Height = height;
		m_windowProperties.Name = name;
	}

	AppWindow::AppWindow(AppWindowProperties& properties)
		: m_NativeWindow(nullptr)
	{
		m_windowProperties = properties;
	}

	AppWindow::~AppWindow()
	{
	}

	void AppWindow::init(RenderingAPI* renderingAPI)
	{
		m_RenderingAPI = renderingAPI;

		glfwInit();
		initVulkan();
	}

	void AppWindow::cleanup()
	{
		glfwDestroyWindow(m_NativeWindow);
		glfwTerminate();
	}

	bool AppWindow::windowShouldClose()
	{
		return glfwWindowShouldClose(m_NativeWindow);
	}

	void AppWindow::PoolEvents()
	{
		glfwPollEvents();
	}

	void AppWindow::OnWindowResized(uint32_t windowWidth, uint32_t windowHeight)
	{
		m_windowProperties.Width = windowWidth;
		m_windowProperties.Height = windowHeight;
		m_RenderingAPI->OnWindowResized(windowWidth, windowHeight);
		m_RenderingAPI->drawFrame();
	}

	void AppWindow::initVulkan()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_NativeWindow = glfwCreateWindow(m_windowProperties.Width, m_windowProperties.Height, "Vulkan", nullptr, nullptr);
		glfwSetWindowUserPointer(m_NativeWindow, this);

		glfwSetFramebufferSizeCallback(m_NativeWindow, framebufferResizeCallback);
	}
}