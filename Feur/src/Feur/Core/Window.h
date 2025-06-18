#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Feur {
	class Window {
	public:
		Window(int width, int height, std::string name);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		inline bool ShouldClose() { return glfwWindowShouldClose(m_NativeWindow); }
		VkExtent2D GetExtent() { return { static_cast<uint32_t>(m_Width), static_cast<uint32_t>(m_Height) }; };
		bool wasWindowResized() { return m_FrameBufferResized; }
		void resetWindowResizedFlag() { m_FrameBufferResized = false; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		static void frameBufferResizeCallback(GLFWwindow* nativeWindow, int width, int height);
		void initWindow();

	private:
		int m_Width;
		int m_Height;
		bool m_FrameBufferResized = false;

		std::string m_Name;

		GLFWwindow* m_NativeWindow;
	};
}