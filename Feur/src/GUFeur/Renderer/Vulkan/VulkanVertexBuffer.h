#pragma once
#include "GUFeur/Renderer/Core/VertexBuffer.h"
#include "GUFeur/Renderer/Vulkan/Device.h"
#include <vulkan/vulkan.h>


namespace GUFeur {
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, Position);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, Color);

			return attributeDescriptions;
		}

	public:
		VulkanVertexBuffer();
		VulkanVertexBuffer(std::vector<Vertex>& vertices);
		~VulkanVertexBuffer() {};

		virtual void createBuffer(Device& device);
		virtual void cleanBuffer(Device& device);
		virtual void bindBuffer(VkCommandBuffer& commandBuffer);


	private:
		VkBuffer m_VertexBuffer; 
		VkDeviceMemory m_VertexBufferMemory;

	};
}