#include "fpch.h"
#include "VulkanVertexBuffer.h"


#include "VulkanAllocator.h"

namespace GUFeur {


	VulkanVertexBuffer::VulkanVertexBuffer()
		: VertexBuffer()
	{
	}

	VulkanVertexBuffer::VulkanVertexBuffer(std::vector<Vertex>& vertices)
		: VertexBuffer(vertices)
	{
	}

	void VulkanVertexBuffer::createVertexBuffer(Device& device)
	{
		VkDeviceSize bufferSize = sizeof(m_Vertices[0]) * m_VertexCount;


		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, device, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, m_Vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(device.device(), stagingBufferMemory);

		createBuffer(bufferSize, device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

		copyBuffer(device, stagingBuffer, m_VertexBuffer, bufferSize);

		vkDestroyBuffer(device.device(), stagingBuffer, GetCallback());
		vkFreeMemory(device.device(), stagingBufferMemory, GetCallback());
	}

	void VulkanVertexBuffer::copyBuffer(Device& device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = device.getTransferCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device.device(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0; // Optionnel
		copyRegion.dstOffset = 0; // Optionnel
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(device.getTransferQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(device.getTransferQueue());

		vkFreeCommandBuffers(device.device(), device.getTransferCommandPool(), 1, &commandBuffer);
	}

	void VulkanVertexBuffer::cleanVertexBuffer(Device& device)
	{
		vkDestroyBuffer(device.device(), m_VertexBuffer, GetCallback());
		vkFreeMemory(device.device(), m_VertexBufferMemory, GetCallback());
	}

	void VulkanVertexBuffer::bindBuffer(VkCommandBuffer& commandBuffer)
	{
		VkBuffer vertexBuffers[] = { m_VertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	}

	void VulkanVertexBuffer::createBuffer(VkDeviceSize size, Device& device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		QueueFamilyIndices indices = device.findPhysicalQueueFamilies();
		uint32_t queueIndices[2]{ indices.graphicsFamily.value(), indices.transferFamily.value() };

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		bufferInfo.queueFamilyIndexCount = 2;
		bufferInfo.pQueueFamilyIndices = queueIndices;

		if (vkCreateBuffer(device.device(), &bufferInfo, GetCallback(), &buffer) != VK_SUCCESS) {
			throw std::runtime_error("echec de la creation d'un vertex buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.device(), buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device.device(), &allocInfo, GetCallback(), &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("echec d'une allocation de memoire!");
		}

		vkBindBufferMemory(device.device(), buffer, bufferMemory, 0);
	}

}