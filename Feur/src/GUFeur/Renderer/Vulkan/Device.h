#pragma once


#include <optional>
#include "GUFeur/Core/Core.h"

#include <vulkan/vulkan.h>



namespace GUFeur {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> transferFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

	};

	class Device
	{
	public:
		Device();
		~Device() {}

		GUF_DELETE_COPY_CONSTRUCTORS(Device)

		void init();
		void cleanup();

		VkDevice device() { return m_Device; }
		VkInstance instance() { return m_Instance; }
		VkSurfaceKHR surface() { return m_Surface; }


		void getSwapchainSupport(SwapChainSupportDetails& supportDetails) { querySwapChainSupport(m_PhysicalDevice, supportDetails); };
		QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(m_PhysicalDevice); };
		VkCommandPool getTransferCommandPool() { return m_TransferCommandPool; }
		VkCommandPool getGraphicsCommandPool() { return m_GraphicsCommandPool; }
		VkQueue getGraphicQueue() { return m_GraphicsQueue; }
		VkQueue getPresentQueue() { return m_PresentQueue; }
		VkQueue getTransferQueue() { return m_TransferQueue; }

		void createGraphicCommandPool();
		void createTransferCommandPool();
		void cleanCommandPools();

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	private:


#pragma region Creation
		void createInstance();
		void setupDebugMessenger();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();
#pragma endregion

#pragma region Cleanup
		void cleanLogicalDevice();
		void cleanPhysicalDevice();
		void cleanSurface();
		void cleanDebugMessenger();
		void cleanInstance();
#pragma endregion

		bool checkValidationLayerSupport();
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		std::vector<const char*> getRequiredExtensions();

		bool isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		void querySwapChainSupport(VkPhysicalDevice device, SwapChainSupportDetails& supportDetails);


	private:
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT m_Callback = VK_NULL_HANDLE;

		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device = VK_NULL_HANDLE;

		VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

		VkQueue m_PresentQueue = VK_NULL_HANDLE;
		VkQueue m_TransferQueue = VK_NULL_HANDLE;

		const std::vector<const char*> m_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> m_DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

#ifdef NDEBUG
		const bool m_EnableValidationLayers = false;
#else
		const bool m_EnableValidationLayers = true;
#endif

		VkCommandPool m_TransferCommandPool = VK_NULL_HANDLE;
		VkCommandPool m_GraphicsCommandPool = VK_NULL_HANDLE;
	};
}

