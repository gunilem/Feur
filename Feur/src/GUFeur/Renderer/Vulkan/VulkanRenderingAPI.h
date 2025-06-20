#pragma once
#include "GUFeur/Core/RenderingAPI.h"
#include "Device.h"
#include "Swapchain.h"

#include "GUFeur/Renderer/Vulkan/VulkanVertexBuffer.h"

#include "VulkanMemoryAllocator.h"


 
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

		virtual VertexBuffer* createVertexBuffer(std::vector<Vertex>& vertices) override;
		virtual void cleanVertexBuffer(VertexBuffer* buffer) override;
		virtual void bindBuffer(VertexBuffer* buffer, VkCommandBuffer& commandBuffer);


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

		VertexBuffer* m_VertexBuffer;

		std::vector < Vertex > vertices = {
			{{1.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		VulkanMemoryAllocator m_VertexBufferMemory;

	};
}

