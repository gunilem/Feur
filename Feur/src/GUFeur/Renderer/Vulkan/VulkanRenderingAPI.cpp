#include "fpch.h"
#include "VulkanRenderingAPI.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



#include "VulkanAllocator.h"




namespace GUFeur {


	VulkanRenderingAPI::VulkanRenderingAPI()
		: m_Device{}, m_Swapchain{m_Device}, m_VertexBufferMemory(m_Device), m_IndexBufferMemory(m_Device)
	{
		SetupAllocator();
	}

	VulkanRenderingAPI::~VulkanRenderingAPI()
	{
		std::cout << "Vulkan allocation : " << GetState()->totalAllocated << "\n";
		CleanAllocator();
	}

	void VulkanRenderingAPI::init(uint32_t windowWidth, uint32_t windowHeight)
	{
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;

		createDevice();
		createSwapchain();
		createCommandBuffers();

		m_VertexBuffer = createVertexBuffer(m_Vertices);
		m_IndexBuffer = createIndexBuffer(m_Indices);
	}

	void VulkanRenderingAPI::cleanup()
	{
		vkDeviceWaitIdle(m_Device.device());

		cleanVertexBuffer(m_VertexBuffer);
		m_VertexBufferMemory.FreeMemory();

		cleanIndexBuffer(m_IndexBuffer);
		m_IndexBufferMemory.FreeMemory();

		cleanCommandBuffers();
		cleanSwapchain();
		cleanDevice();
	}

	void VulkanRenderingAPI::drawFrame()
	{
		if (m_windowWidth <= 0 || m_windowHeight <= 0) return;

		uint32_t imageIndex = 0;
		VkResult result = m_Swapchain.acquireNextImage(&imageIndex);

		m_Swapchain.updateUniformBuffer(imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("échec de la présentation d'une image à la swap chain!");
		}

		recordCommandBuffers(imageIndex);

		result = m_Swapchain.submitCommandBuffer(&m_CommandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_WindowResized) {
			m_WindowResized = false;
			recreateSwapchain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("échec de la présentation d'une image!");
		}
	}

	void VulkanRenderingAPI::OnWindowResized(uint32_t windowWidth, uint32_t windowHeight)
	{ 
		m_WindowResized = true; 
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;
		recreateSwapchain();
	}

#pragma region Creation

	void VulkanRenderingAPI::createDevice()
	{
		m_Device.init();
	}

	void VulkanRenderingAPI::createSwapchain()
	{
		m_Swapchain.init(m_windowWidth, m_windowHeight);
	}

	void VulkanRenderingAPI::createCommandBuffers()
	{
		m_CommandBuffers.resize(m_Swapchain.getImageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_Device.getGraphicsCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

		if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("échec de l'allocation de command buffers!");
		}
	}
#pragma endregion

#pragma region Cleanup

	void VulkanRenderingAPI::cleanCommandBuffers()
	{
		vkFreeCommandBuffers(m_Device.device(), m_Device.getGraphicsCommandPool(), static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
		vkResetCommandPool(m_Device.device(), m_Device.getGraphicsCommandPool(), 0);
		
		m_CommandBuffers.clear();
	}

	void VulkanRenderingAPI::cleanSwapchain()
	{
		m_Swapchain.cleanup();
	}

	void VulkanRenderingAPI::cleanDevice()
	{
		m_Device.cleanup();
	}
#pragma endregion

	void VulkanRenderingAPI::recordCommandBuffers(uint32_t imageIndex)
	{
		vkResetCommandBuffer(m_CommandBuffers[imageIndex], NULL);
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optionnel
		beginInfo.pInheritanceInfo = nullptr; // Optionel

		if (vkBeginCommandBuffer(m_CommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("erreur au début de l'enregistrement d'un command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_Swapchain.getRenderPass();
		renderPassInfo.framebuffer = m_Swapchain.getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_Swapchain.getExtents();

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(m_CommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		m_Swapchain.getGraphicPipeline()->bind(m_CommandBuffers[imageIndex]);

		bindBuffer(m_VertexBuffer, m_CommandBuffers[imageIndex]);
		bindBuffer(m_IndexBuffer, m_CommandBuffers[imageIndex]);

		m_Swapchain.bindDescriptorSet(m_CommandBuffers[imageIndex], imageIndex);
		vkCmdDrawIndexed(m_CommandBuffers[imageIndex], static_cast<uint32_t>(m_IndexBuffer->bufferDataCount()), 1, 0, 0, 0);

		vkCmdEndRenderPass(m_CommandBuffers[imageIndex]);
		if (vkEndCommandBuffer(m_CommandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("échec de l'enregistrement d'un command buffer!");
		}

	}

	void VulkanRenderingAPI::recreateSwapchain()
	{
		if (m_windowWidth <= 0 || m_windowHeight <= 0) return;

		vkDeviceWaitIdle(m_Device.device());

		m_Device.cleanCommandPools();

		m_Device.createGraphicCommandPool();
		m_Device.createTransferCommandPool();
		m_Swapchain.recreateSwapchain(m_windowWidth, m_windowHeight, m_Swapchain.getSwapchain());

		createCommandBuffers();
	}

	Buffer<Vertex>* VulkanRenderingAPI::createVertexBuffer(std::vector<Vertex>& vertices)
	{
		VulkanBuffer<Vertex>* buffer = new VulkanBuffer<Vertex>{ BufferTypes::Vertex, vertices.data(), vertices.size(), m_VertexBufferMemory};

		buffer->InitBuffer(m_Device);

		return dynamic_cast<VulkanBuffer<Vertex>*>(buffer);
	}

	Buffer<uint16_t>* VulkanRenderingAPI::createIndexBuffer(std::vector<uint16_t>& indices)
	{
		VulkanBuffer<uint16_t>* buffer = new VulkanBuffer<uint16_t>{ BufferTypes::Index, indices.data(), indices.size(), m_IndexBufferMemory};

		buffer->InitBuffer(m_Device);

		return dynamic_cast<VulkanBuffer<uint16_t>*>(buffer);
	}

	void VulkanRenderingAPI::cleanVertexBuffer(Buffer<Vertex>* buffer)
	{
		VulkanBuffer<Vertex>* b = dynamic_cast<VulkanBuffer<Vertex>*>(buffer);
		b->cleanBuffer(m_Device);
		delete b;
	}

	void VulkanRenderingAPI::cleanIndexBuffer(Buffer<uint16_t>* buffer)
	{
		VulkanBuffer<uint16_t>* b = dynamic_cast<VulkanBuffer<uint16_t>*>(buffer);
		b->cleanBuffer(m_Device);
		delete b;
	}

	template<typename T>
	void VulkanRenderingAPI::bindBuffer(Buffer<T>* buffer, VkCommandBuffer& commandBuffer)
	{
		VulkanBuffer<T>* b = dynamic_cast<VulkanBuffer<T>*>(buffer);
		b->bindBuffer(commandBuffer);
	}
}