#pragma once

#include "Core.h"
#include "RenderingAPI.h"
#include "AppWindow.h"

#include <chrono>

namespace GUFeur {

	struct AppProperties {
		std::chrono::steady_clock::time_point timeLastDisplayed;
		float frameRate;

		void DisplayInfo(){
			if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - timeLastDisplayed).count() < 1) return;

			timeLastDisplayed = std::chrono::high_resolution_clock::now();

			std::cout << "FPS : " << frameRate << "\n";
		}
	};

	class Application
	{
	public:
		static Application* get() { return m_Application; };

	public:
		Application();
		~Application();

		GUF_DELETE_COPY_CONSTRUCTORS(Application)

		virtual void run();
		virtual AppWindow& window() { return m_AppWindow; }

	private:
		void initWindow();
		void initRenderingApi();
		void mainLoop();
		void cleanup();

		void drawFrame();

	private:
		AppWindow m_AppWindow;
		RenderingAPI* m_RenderingAPI;
		AppProperties m_AppProperties;

	private:
		static Application* m_Application;
	};
}