#pragma once

#include "Core.h"
#include "RenderingAPI.h"
#include "AppWindow.h"


namespace GUFeur {
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

	private:
		static Application* m_Application;
	};
}