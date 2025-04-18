#pragma once

#include "Feur/UI/Widget.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Feur {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	class Application
	{
	public:
		static Application* Create(const std::string& name);

		void Run();
		int Init();

		void InitVukan();
		void CreateSurface();
		void PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
		bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
		void CreateLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

	private:
		Application(const std::string& name);

	private:
		std::string m_ApplicationName;

		GLFWwindow* m_NativeWindow;
		std::shared_ptr<Widget> m_Rootwidget;


		VkInstance instance;

		VkSurfaceKHR surface;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
	};
}

