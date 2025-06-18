#include "fpch.h"
#include "Application.h"

#include <stdlib.h>  
#include <crtdbg.h>  


namespace GUFeur {

	


	Application* Application::m_Application = nullptr;;

	Application::Application()
		: m_AppWindow(800, 600, "Vulkan")
	{
		m_RenderingAPI = RenderingAPI::create();
		m_Application = this;
	}

	Application::~Application() {
	}


	void Application::run()
	{
		initWindow();
		initRenderingApi();
		mainLoop();
		cleanup();

	}

	void Application::initWindow() {
		m_AppWindow.init(m_RenderingAPI);
	}

	void Application::initRenderingApi() {
		m_RenderingAPI->init(m_AppWindow.GetWindowProperties().Width, m_AppWindow.GetWindowProperties().Height);
	}

	void Application::mainLoop() {
		while (!m_AppWindow.windowShouldClose()) {
			m_AppWindow.PoolEvents();
			drawFrame();
		}
	}

	void Application::cleanup() {
		m_RenderingAPI->cleanup();
		delete m_RenderingAPI;
		m_AppWindow.cleanup();
	}

	void Application::drawFrame()
	{
		m_RenderingAPI->drawFrame();
	}
}