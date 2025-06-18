#pragma once

#include "GUFeur/Core/Core.h"
#include "RenderingAPI.h"

struct GLFWwindow;

namespace GUFeur {

	struct AppWindowProperties {
		uint32_t Width;
		uint32_t Height;
		const char* Name;
	};

	class AppWindow
	{
	public:
		AppWindow(uint32_t width, uint32_t height, const char* name);
		AppWindow(AppWindowProperties& properties);
		virtual ~AppWindow();

		GUF_DELETE_COPY_CONSTRUCTORS(AppWindow)

		virtual void init(RenderingAPI* renderingAPI);
		virtual void cleanup();

		virtual bool windowShouldClose();
		virtual void PoolEvents();

		virtual GLFWwindow* nativeWindow() { return m_NativeWindow; }
		virtual void OnWindowResized(uint32_t windowWidth, uint32_t windowHeight);

		virtual AppWindowProperties GetWindowProperties() { return m_windowProperties; }

	private:
		virtual void initVulkan();

	private:
		GLFWwindow* m_NativeWindow = nullptr;
		AppWindowProperties m_windowProperties;

		RenderingAPI* m_RenderingAPI = nullptr;
	};
}
