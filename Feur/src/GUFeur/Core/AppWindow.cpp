#include "fpch.h"
#include "AppWindow.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "GUFeur/Renderer/Core/Model.h"

namespace GUFeur {


	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
		app->OnWindowResized(width, height);
	}

	AppWindow::AppWindow(uint32_t width, uint32_t height, const char* name)
		: m_UIData(width, height, 0.0f, 0.0f)
	{
		m_windowProperties = AppWindowProperties();
		m_windowProperties.Width = width;
		m_windowProperties.Height = height;
		m_windowProperties.Name = name;

		m_RootUIElement = new TabLayout{ };
	}

	AppWindow::AppWindow(AppWindowProperties& properties)
		: m_NativeWindow(nullptr), m_UIData(properties.Width, properties.Height, 0.0f, 0.0f)
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

	void AppWindow::start()
	{
		m_RootUIElement->update(m_RenderingAPI, m_UIData);
	}

	void AppWindow::stop()
	{
		m_RootUIElement->cleanup(m_RenderingAPI);

		delete m_RootUIElement;
	}

	bool AppWindow::windowShouldClose()
	{
		return glfwWindowShouldClose(m_NativeWindow);
	}

	void AppWindow::PoolEvents()
	{
		glfwPollEvents();
	}

	void AppWindow::drawFrame()
	{
		m_RootUIElement->draw(m_RenderingAPI);
	}

	void AppWindow::OnWindowResized(uint32_t windowWidth, uint32_t windowHeight)
	{
		m_windowProperties.Width = windowWidth;
		m_UIData.Width = windowWidth;
		m_windowProperties.Height = windowHeight;
		m_UIData.Height = windowHeight;
		m_RenderingAPI->OnWindowResized(windowWidth, windowHeight);

		m_RootUIElement->update(m_RenderingAPI, m_UIData);
	}

	void AppWindow::initVulkan()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_NativeWindow = glfwCreateWindow(m_windowProperties.Width, m_windowProperties.Height, "Vulkan", nullptr, nullptr);
		glfwSetWindowUserPointer(m_NativeWindow, this);

		glfwSetFramebufferSizeCallback(m_NativeWindow, framebufferResizeCallback);
	}
}