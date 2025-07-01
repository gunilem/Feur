#pragma once
#include "GUFeur/Core/RenderingAPI.h"

#include "VulkanMemoryAllocator.h"
#include "VulkanVertexBuffer.h"


#include "Swapchain.h"
#include "Device.h"

 
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

		virtual Buffer<Vertex>* createVertexBuffer(std::vector<Vertex>& vertices) override;
		virtual Buffer<uint16_t>* createIndexBuffer(std::vector<uint16_t>& indices) override;

		virtual void cleanVertexBuffer(Buffer<Vertex>* buffer) override;
		virtual void cleanIndexBuffer(Buffer<uint16_t>* buffer) override;

		template<typename T>
		void bindBuffer(Buffer<T>* buffer, VkCommandBuffer& commandBuffer);


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

		Buffer<Vertex>* m_VertexBuffer;
		Buffer<uint16_t>* m_IndexBuffer;

		std::vector < Vertex > m_Vertices = {
			{{0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{{800.f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{800.f, 1000.0f}, {0.0f, 0.0f, 1.0f}},
			{{0.0f, 1000.0f}, {1.0f, 1.0f, 1.0f}}
		};

		std::vector<uint16_t> m_Indices = {
			0, 1, 2, 2, 3, 0
		};


		VulkanMemoryAllocator m_VertexBufferMemory;
		VulkanMemoryAllocator m_IndexBufferMemory;
	};
}

