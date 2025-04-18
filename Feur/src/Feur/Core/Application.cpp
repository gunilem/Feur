#include "fpch.h"
#include "Application.h"

#include "Feur/UI/Window.h"
#include "Feur/UI/Button.h"




namespace Feur {

	

	Application* Application::Create(const std::string& name)
	{

		return new Application(name);
	}
	void Application::Run()
	{
		while (!glfwWindowShouldClose(m_NativeWindow)) {
			glfwPollEvents();

			m_Rootwidget->draw();
		}

		glfwDestroyWindow(m_NativeWindow);
		glfwTerminate();
	}

	int Application::Init()
	{
		if (!glfwInit()) {
			std::cerr << "Failed to Initialize GLFW" << std::endl;
			return -1;
		}

		if (!glfwVulkanSupported()) {
			std::cerr << "Vulkan is not supported" << std::endl;
			return -1;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_NativeWindow = glfwCreateWindow(1280, 720, m_ApplicationName.c_str(), nullptr, nullptr);
		if (!m_NativeWindow) {
			std::cerr << "Failed to Create a Window" << std::endl;
			glfwTerminate();
			return -1;
		}

		m_Rootwidget = std::make_shared<Window>("Feur");

		std::shared_ptr<Button> b1 = std::make_shared<Button>("Click me");
		std::shared_ptr<Button> b2 = std::make_shared<Button>("Exit");

		m_Rootwidget->addChild(b1);
		m_Rootwidget->addChild(b2);

		InitVukan();
		CreateSurface();

		return 0;
	}

	Application::Application(const std::string& name) : m_ApplicationName(name)
	{
		
	}

	void Application::InitVukan() {
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Feur";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "FeurEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		// Get required extensions from GLFW
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		// Instance creation info
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0; // add validation layers here if needed

		// Create instance
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Vulkan instance!");
		}
	}

	void Application::CreateSurface() {
		if (glfwCreateWindowSurface(instance, m_NativeWindow, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface!");
		}
	}


	void PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			throw std::runtime_error("No Vulkan-supported GPUs found!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		// Choose the first suitable device
		for (const auto& device : devices) {
			if (IsDeviceSuitable(device, surface)) {
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("No suitable GPU found!");
		}
	}

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
		QueueFamilyIndices indices;

		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

		std::vector<VkQueueFamilyProperties> families(count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, families.data());

		int i = 0;
		for (const auto& family : families) {
			if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) break;
			i++;
		}

		return indices;
	}

	bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
		QueueFamilyIndices indices = FindQueueFamilies(device, surface);
		return indices.isComplete();
	}

	void CreateLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice, surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueues = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueues) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{}; // enable features if needed

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;

		// Extensions for swapchain
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create logical device!");
		}

		// Get handles to queues
		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
	}
}