#pragma once
#include "GUFeur/Core/RenderingAPI.h"
#include "Device.h"
#include "Swapchain.h"


 
namespace GUFeur {
	
	class VulkanRenderingAPI : public RenderingAPI
	{
	public:
		VulkanRenderingAPI();
		virtual ~VulkanRenderingAPI() override;

		GUF_DELETE_COPY_CONSTRUCTORS(VulkanRenderingAPI)

		virtual void init(uint32_t windowWidth, uint32_t windowHeight) override;
		virtual void cleanup() override;

		virtual void drawFrame() override;

		virtual void OnWindowResized(uint32_t windowWidth, uint32_t windowHeight) override;

	private:
		void recordCommandBuffers(uint32_t imageIndex);
		void recreateSwapchain();


#pragma region Creation
		void createDevice();
		void createSwapchain();
		void createCommandBuffers();
#pragma endregion

#pragma region Cleanup
		void cleanCommandBuffers();
		void cleanSwapchain();
		void cleanDevice();
#pragma endregion


	private:
		Device m_Device;
		Swapchain m_Swapchain;

		std::vector<VkCommandBuffer> m_CommandBuffers;
		uint32_t m_windowWidth, m_windowHeight;
		bool m_WindowResized;

	};
}

