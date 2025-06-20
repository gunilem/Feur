#include "fpch.h"
#include "Application.h"

#include <stdlib.h>  
#include <crtdbg.h>  



namespace GUFeur {

	


	Application* Application::m_Application = nullptr;;

	Application::Application()
		: m_AppWindow(800, 600, "Vulkan"), m_AppProperties{}
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
			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now());
			m_AppWindow.PoolEvents();
			drawFrame();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now());

			m_AppProperties.frameRate = 1000000 / std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			m_AppProperties.DisplayInfo();
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