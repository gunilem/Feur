﻿#include "fpch.h"
#include "Swapchain.h"

#include <cstdint> // uint32_t
#include <limits> // std::numeric_limits
#include <algorithm> // std::clamp
#include <ctime>


#include "VulkanAllocator.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>


#undef max

//TODO Remove this
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

namespace GUFeur {
	Swapchain::Swapchain(Device& deviceRef)
		: m_Device(deviceRef), m_UniformBufferMemory(m_Device)
	{

	}

	void Swapchain::init(uint32_t width, uint32_t height)
	{
		createSwapChain(width, height);
		createImageViews();
		createRenderPass();
		createDescriptorSetLayout();

		createUniformBuffers(m_UniformBuffers);
		createDescriptorPool();
		createDescriptorSets();

		createGraphicsPipeline();
		createFramebuffers();
		createSyncObjects();

	}

	void Swapchain::cleanup()
	{
		cleanSyncObjects();
		cleanFramebuffers();
		cleanGraphicsPipeline();
		cleanRenderPass();
		cleanImageViews();
		cleanSwapChain();

		cleanUniformBuffers(m_UniformBuffers);
		m_UniformBufferMemory.FreeMemory();
		cleanDescriptorPool();
		cleanDescriptorSetLayout();
	}

	VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D Swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t newWidth, uint32_t newHeight)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			VkExtent2D actualExtent = { newWidth, newHeight };

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);


			return actualExtent;
		}
	}

	void Swapchain::updateUniformBuffer(uint32_t currentImage)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo{};
		ubo.model = glm::mat4(1.0f);
		ubo.view = glm::mat4(1.0f);

		ubo.proj = glm::ortho(
			0.0f, (float)getExtents().width,
			0.0f, (float)getExtents().height,
			-1.0f, 1.0f
		);

		VulkanBuffer<UniformBufferObject>* buffer = dynamic_cast<VulkanBuffer<UniformBufferObject>*>(m_UniformBuffers[currentImage]);
		buffer->MoveData(m_Device, &ubo, 1);
	}

	void Swapchain::bindDescriptorSet(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[imageIndex], 0, nullptr);
	}


#pragma region Creation

	void Swapchain::createSwapChain(uint32_t width, uint32_t height)
	{
		SwapChainSupportDetails swapChainSupport;
		m_Device.getSwapchainSupport(swapChainSupport);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		m_SwapchainImageFormat = surfaceFormat.format;
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		m_SwapchainImageExtent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

		m_SwapchainImageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && m_SwapchainImageCount > swapChainSupport.capabilities.maxImageCount) {
			m_SwapchainImageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Device.surface();
		createInfo.minImageCount = m_SwapchainImageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = m_SwapchainImageExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = m_Device.findPhysicalQueueFamilies();
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optionnel
			createInfo.pQueueFamilyIndices = nullptr; // Optionnel
		}
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = m_PreviousSwapchain;


		if (vkCreateSwapchainKHR(m_Device.device(), &createInfo,GetCallback(), &m_Swapchain) != VK_SUCCESS) {
			throw std::runtime_error("échec de la création de la swap chain!");
		}

		vkGetSwapchainImagesKHR(m_Device.device(), m_Swapchain, &m_SwapchainImageCount, nullptr);
		m_SwapchainImages.resize(m_SwapchainImageCount);
		vkGetSwapchainImagesKHR(m_Device.device(), m_Swapchain, &m_SwapchainImageCount, m_SwapchainImages.data());

	}

	void Swapchain::createImageViews()
	{
		m_SwapchainImageViews.resize(m_SwapchainImages.size());

		for (size_t i = 0; i < m_SwapchainImages.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_SwapchainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_SwapchainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(m_Device.device(), &createInfo,GetCallback(), &m_SwapchainImageViews[i]) != VK_SUCCESS) {
				throw std::runtime_error("échec de la création d'une image view!");
			}
		}
	}

	void Swapchain::createRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_SwapchainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;


		if (vkCreateRenderPass(m_Device.device(), &renderPassInfo,GetCallback(), &m_RenderPass) != VK_SUCCESS) {
			throw std::runtime_error("échec de la création de la render pass!");
		}
	}

	void Swapchain::createDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optionnel

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		if (vkCreateDescriptorSetLayout(m_Device.device(), &layoutInfo, GetCallback(), &m_DescriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("echec de la creation d'un set de descripteurs!");
		}
	}

	void Swapchain::createGraphicsPipeline()
	{

		GraphicPipelineCreateInfo graphicPipelineInfo{};
		GraphicPipeline::defaultGraphicPipelineConfigInfo(graphicPipelineInfo);


		auto bindingDescription = VulkanBuffer<Vertex>::getBindingDescription();
		auto attributeDescriptions = VulkanBuffer<Vertex>::getAttributeDescriptions();

		graphicPipelineInfo.VertexInputInfo.vertexBindingDescriptionCount = 1;
		graphicPipelineInfo.VertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		graphicPipelineInfo.VertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		graphicPipelineInfo.VertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		graphicPipelineInfo.RenderPass = m_RenderPass;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)m_SwapchainImageExtent.width;
		viewport.height = (float)m_SwapchainImageExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapchainImageExtent;

		graphicPipelineInfo.ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		graphicPipelineInfo.ViewportInfo.viewportCount = 1;
		graphicPipelineInfo.ViewportInfo.pViewports = &viewport;
		graphicPipelineInfo.ViewportInfo.scissorCount = 1;
		graphicPipelineInfo.ViewportInfo.pScissors = &scissor;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;            // Optionnel
		pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;         // Optionnel
		pipelineLayoutInfo.pushConstantRangeCount = 0;    // Optionnel
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optionnel


		if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, GetCallback(), &m_PipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("échec de la création du pipeline layout!");
		}

		graphicPipelineInfo.PipelineLayout = m_PipelineLayout;


		m_GraphicPipeline = new GraphicPipeline(m_Device, graphicPipelineInfo);
		m_GraphicPipeline->init();
	}

	void Swapchain::createFramebuffers()
	{
		m_SwapchainFramebuffers.resize(m_SwapchainImageViews.size());

		for (size_t i = 0; i < m_SwapchainImageViews.size(); i++) {
			VkImageView attachments[] = {
				m_SwapchainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_RenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_SwapchainImageExtent.width;
			framebufferInfo.height = m_SwapchainImageExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(m_Device.device(), &framebufferInfo,GetCallback(), &m_SwapchainFramebuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("échec de la création d'un framebuffer!");
			}
		}
	}

	void Swapchain::createSyncObjects()
	{
		m_ImageAvailableSemaphores.resize(m_SwapchainImageCount);
		m_RenderFinishedSemaphores.resize(m_SwapchainImageCount);
		m_InFlightFences.resize(m_SwapchainImageCount);
		m_ImagesInFlight.resize(m_SwapchainImages.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;


		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < m_SwapchainImageCount; i++) {
			if (vkCreateSemaphore(m_Device.device(), &semaphoreInfo,GetCallback(), &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(m_Device.device(), &semaphoreInfo,GetCallback(), &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(m_Device.device(), &fenceInfo,GetCallback(), &m_InFlightFences[i]) != VK_SUCCESS) {

				throw std::runtime_error("échec de la création des objets de synchronisation pour une frame!");
			}
		}
	}
	void Swapchain::createUniformBuffers(std::vector<Buffer<UniformBufferObject>*>& buffers)
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		buffers.resize(m_SwapchainImageCount);

		UniformBufferObject obj{};

		for (size_t i = 0; i < m_SwapchainImageCount; i++) {
			VulkanBuffer<UniformBufferObject>* buffer = new VulkanBuffer<UniformBufferObject>{ BufferTypes::Uniform, &obj, 1, m_UniformBufferMemory };
			buffer->InitBuffer(m_Device);
			buffers[i] = buffer;
		}
	}
	void Swapchain::createDescriptorPool()
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<uint32_t>(m_SwapchainImageCount);

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = static_cast<uint32_t>(m_SwapchainImageCount);

		if (vkCreateDescriptorPool(m_Device.device(), &poolInfo, GetCallback(), &m_DescriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("echec de la creation de la pool de descripteurs!");
		}
	}

	void Swapchain::createDescriptorSets()
	{
		std::vector<VkDescriptorSetLayout> layouts(m_SwapchainImageCount, m_DescriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(m_SwapchainImageCount);
		allocInfo.pSetLayouts = layouts.data();

		m_DescriptorSets.resize(m_SwapchainImageCount);
		if (vkAllocateDescriptorSets(m_Device.device(), &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS) {
			throw std::runtime_error("echec de l'allocation d'un set de descripteurs!");
		}

		for (size_t i = 0; i < m_SwapchainImageCount; i++) {

			VulkanBuffer<UniformBufferObject>* b = dynamic_cast<VulkanBuffer<UniformBufferObject>*>(m_UniformBuffers[i]);

			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = b->getBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr; // Optionnel
			descriptorWrite.pTexelBufferView = nullptr; // Optionnel

			vkUpdateDescriptorSets(m_Device.device(), 1, &descriptorWrite, 0, nullptr);
		}
	}
#pragma endregion


#pragma region Cleanup

	void Swapchain::cleanSwapChain()
	{
		vkDestroySwapchainKHR(m_Device.device(), m_Swapchain,GetCallback());
	}

	void Swapchain::cleanImageViews()
	{
		for (size_t i = 0; i < m_SwapchainImageViews.size(); i++) {
			vkDestroyImageView(m_Device.device(), m_SwapchainImageViews[i],GetCallback());
		}

		m_SwapchainImageViews.clear();
	}

	void Swapchain::cleanRenderPass()
	{
		vkDestroyRenderPass(m_Device.device(), m_RenderPass,GetCallback());
	}

	void Swapchain::cleanGraphicsPipeline()
	{
		m_GraphicPipeline->cleanup();
		delete m_GraphicPipeline;
		vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout,GetCallback());
	}

	void Swapchain::cleanDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(m_Device.device(), m_DescriptorSetLayout, GetCallback());
	}

	void Swapchain::cleanFramebuffers()
	{
		for (size_t i = 0; i < m_SwapchainFramebuffers.size(); i++) {
			vkDestroyFramebuffer(m_Device.device(), m_SwapchainFramebuffers[i],GetCallback());
		}

		m_SwapchainFramebuffers.clear();
	}

	void Swapchain::cleanSyncObjects()
	{
		for (size_t i = 0; i < m_SwapchainImageCount; i++) {
			vkDestroySemaphore(m_Device.device(), m_RenderFinishedSemaphores[i],GetCallback());
			vkDestroySemaphore(m_Device.device(), m_ImageAvailableSemaphores[i],GetCallback());
			vkDestroyFence(m_Device.device(), m_InFlightFences[i],GetCallback());
		}
	}

	void Swapchain::cleanDescriptorSets()
	{
		vkDestroyDescriptorSetLayout(m_Device.device(), m_DescriptorSetLayout, GetCallback());
	}

	void Swapchain::cleanDescriptorPool()
	{
		vkDestroyDescriptorPool(m_Device.device(), m_DescriptorPool, GetCallback());
	}

	void Swapchain::cleanUniformBuffers(std::vector<Buffer<UniformBufferObject>*>& buffers)
	{
		for (int i = m_SwapchainImageCount - 1; i >= 0; i--)
		{
			VulkanBuffer<UniformBufferObject>* b = dynamic_cast<VulkanBuffer<UniformBufferObject>*>(buffers[i]);
			b->cleanBuffer(m_Device);
			delete b;
		}
	}

#pragma endregion


	VkResult Swapchain::acquireNextImage(uint32_t* nextImage)
	{
		vkWaitForFences(m_Device.device(), 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

		return vkAcquireNextImageKHR(m_Device.device(), m_Swapchain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, nextImage);
	}

	VkResult Swapchain::submitCommandBuffer(const VkCommandBuffer* buffers, uint32_t* imageIndex)
	{
		
		if (m_ImagesInFlight[*imageIndex] != VK_NULL_HANDLE) {
			vkWaitForFences(m_Device.device(), 1, &m_ImagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
		}
		m_ImagesInFlight[*imageIndex] = m_InFlightFences[m_CurrentFrame];


		VkSemaphore aquireSemaphores = m_ImageAvailableSemaphores[m_CurrentFrame];
		VkSemaphore signalSemaphores = m_RenderFinishedSemaphores[m_CurrentFrame];
		VkSwapchainKHR swapChains = m_Swapchain;

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkPipelineStageFlags waitStages =  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT ;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &aquireSemaphores;
		submitInfo.pWaitDstStageMask = &waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = buffers;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &signalSemaphores;

		vkResetFences(m_Device.device(), 1, &m_InFlightFences[m_CurrentFrame]);
		if (vkQueueSubmit(m_Device.getGraphicQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapChains;
		presentInfo.pImageIndices = imageIndex;

		auto result = vkQueuePresentKHR(m_Device.getPresentQueue(), &presentInfo);
		m_CurrentFrame = (m_CurrentFrame + 1) % m_SwapchainImageCount;

		return result;
	}


	void Swapchain::recreateSwapchain(uint32_t newWidth, uint32_t newHeight, VkSwapchainKHR oldSwapchain)
	{
		m_PreviousSwapchain = oldSwapchain;

		cleanFramebuffers();
		cleanGraphicsPipeline();
		cleanRenderPass();
		cleanImageViews();


		createSwapChain(newWidth, newHeight);

		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();

		vkDestroySwapchainKHR(m_Device.device(), m_PreviousSwapchain, GetCallback());
	}

}