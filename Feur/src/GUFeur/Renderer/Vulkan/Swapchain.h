#pragma once

#include "GUFeur/Core/Core.h"

#include "Device.h"
#include "GraphicPipeline.h"

namespace GUFeur {

	class Swapchain
	{
	public:
		Swapchain(Device& deviceRef);
		~Swapchain() {}

		GUF_DELETE_COPY_CONSTRUCTORS(Swapchain)

		void init(uint32_t width, uint32_t height);
		void cleanup();

		VkRenderPass getRenderPass() { return m_RenderPass; }
		uint32_t getImageCount() { return m_SwapchainImageCount; }
		VkFramebuffer getFrameBuffer(int index) { return m_SwapchainFramebuffers[index]; }
		VkExtent2D getExtents() { return m_SwapchainImageExtent; }
		GraphicPipeline* getGraphicPipeline() { return m_GraphicPipeline; }
		VkResult acquireNextImage(uint32_t* nextImage);
		VkResult submitCommandBuffer(const VkCommandBuffer* buffers, uint32_t* imageIndex);

		VkSwapchainKHR getSwapchain() { return m_Swapchain; }

		void recreateSwapchain(uint32_t newWidth, uint32_t newHeight, VkSwapchainKHR oldSwapchain);

	private:
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t newWidth, uint32_t newHeight);

#pragma region Creation
		void createSwapChain(uint32_t width, uint32_t height);
		void createImageViews();

		void createRenderPass();
		void createDescriptorSetLayout();
		void createGraphicsPipeline();
		void createFramebuffers();

		void createSyncObjects();
#pragma endregion

#pragma region Cleanup
		void cleanSwapChain();
		void cleanImageViews();

		void cleanRenderPass();
		void cleanGraphicsPipeline();
		void cleanDescriptorSetLayout();
		void cleanFramebuffers();

		void cleanSyncObjects();
#pragma endregion



	private:
		Device& m_Device;
		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
		VkSwapchainKHR m_PreviousSwapchain = VK_NULL_HANDLE;
		std::vector<VkImage> m_SwapchainImages;

		uint32_t m_SwapchainImageCount;
		VkFormat m_SwapchainImageFormat;
		VkExtent2D m_SwapchainImageExtent;

		std::vector<VkImageView> m_SwapchainImageViews;

		VkRenderPass m_RenderPass = VK_NULL_HANDLE;
		GraphicPipeline* m_GraphicPipeline = nullptr;

		std::vector<VkFramebuffer> m_SwapchainFramebuffers;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;
		std::vector<VkFence> m_ImagesInFlight;

		uint32_t m_CurrentFrame = 0;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;
	};

}
