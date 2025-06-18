#include "fpch.h"
#include "Application.h"


#include <set>
#include <filesystem>
#include <assert.h>

namespace Feur {

	

	Application* Application::Create(const std::string& name)
	{

		return new Application(name);
	}
	void Application::Run()
	{
		while (!m_Window.ShouldClose()) {
			glfwPollEvents();

			DrawFrame();
		}

		vkDeviceWaitIdle(m_Device.device());
	}

	void Application::LoadModels()
	{
		std::vector<Model::Vertex> vertices{
			{{ 0.0f, -0.5f}, {0.8f, 0.2f, 0.3f}},
			{{ 0.5f,  0.5f}, {0.3f, 0.8f, 0.2f}},
			{{-0.5f,  0.5f}, {0.2f, 0.3f, 0.8f}}
		};

		m_Model = std::make_unique<Model>(m_Device, vertices);
	}

	void Application::CreatePipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, NULL, &m_PipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	void Application::CreatePipeline()
	{
		assert(m_SwapChain != nullptr && "Connot create pipeline before swap Chain");
		assert(m_PipelineLayout != nullptr && "Connot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::defaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.RenderPass = m_SwapChain->getRenderPass();
		pipelineConfig.PipelineLayout = m_PipelineLayout;

		m_Pipeline = std::make_unique<Pipeline>(m_Device, 
			"assets/shader.vert.spv",
			"assets/shader.frag.spv",
			pipelineConfig);
	}

	void Application::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(m_SwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_Device.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

		if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers");
		}
	}

	void Application::freeCommandBuffer()
	{
		vkFreeCommandBuffers(
			m_Device.device(), 
			m_Device.getCommandPool(), 
			static_cast<uint32_t>(m_CommandBuffers.size()),
			m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}

	void Application::DrawFrame() {
		uint32_t imageIndex;
		auto result = m_SwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			RecreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to aquire swap chain image");
		}

		RecordCommandBuffer(imageIndex);
		result = m_SwapChain->submitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window.wasWindowResized()) {
			m_Window.resetWindowResizedFlag();
			RecreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}
	}

	void Application::RecreateSwapChain()
	{
		auto extent = m_Window.GetExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = m_Window.GetExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_Device.device());
		if (m_SwapChain == nullptr) {
			m_SwapChain = std::make_unique<SwapChain>(m_Device, extent);
		}
		else {
			m_SwapChain = std::make_unique<SwapChain>(m_Device, extent, std::move(m_SwapChain));
			if (m_SwapChain->imageCount() != m_CommandBuffers.size()) {
				freeCommandBuffer();
				CreateCommandBuffers();
			}
		}
		CreatePipeline();
	}

	void Application::RecordCommandBuffer(int imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(m_CommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain->getRenderPass();
		renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		//clearValues[0].color = { 0.8f, 0.2f, 0.3f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_CommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{ {0, 0}, m_SwapChain->getSwapChainExtent() };
		vkCmdSetViewport(m_CommandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(m_CommandBuffers[imageIndex], 0, 1, &scissor);


		m_Pipeline->bind(m_CommandBuffers[imageIndex]);
		m_Model->Bind(m_CommandBuffers[imageIndex]);
		m_Model->Draw(m_CommandBuffers[imageIndex]);

		vkCmdEndRenderPass(m_CommandBuffers[imageIndex]);
		if (vkEndCommandBuffer(m_CommandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}

	int Application::Init()
	{
		InitWindow();



		return 0;
	}

	void Application::InitWindow() {
		
	}

	void Application::Cleanup() {
		
	}

	Application::Application(const std::string& name) 
		: m_ApplicationName(name),
		m_Window{ WIDTH, HEIGHT, "Vulkan" },
		m_Device{ m_Window }
	{
		LoadModels();
		CreatePipelineLayout();
		RecreateSwapChain();
		CreateCommandBuffers();
	}
	Application::~Application()
	{
		vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
	}
}