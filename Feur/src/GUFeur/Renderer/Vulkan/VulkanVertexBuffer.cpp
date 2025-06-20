#include "fpch.h"
#include "VulkanVertexBuffer.h"


#include "VulkanAllocator.h"

namespace GUFeur {


	VulkanVertexBuffer::VulkanVertexBuffer(VulkanMemoryAllocator& vertexBufferMemory)
		: VertexBuffer(), m_MemoryAllocator(vertexBufferMemory)
	{
	}

	VulkanVertexBuffer::VulkanVertexBuffer(std::vector<Vertex>& vertices, VulkanMemoryAllocator& vertexBufferMemory)
		: VertexBuffer(vertices), m_MemoryAllocator(vertexBufferMemory)
	{
	}

	void VulkanVertexBuffer::createVertexBuffer(Device& device)
	{
		VkDeviceSize bufferSize = sizeof(m_Vertices[0]) * m_VertexCount;


		VkBuffer stagingBuffer;
		Allocation stagingBufferMemory;
		createBuffer(bufferSize, device, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device.device(), stagingBufferMemory.memory, 0, bufferSize, 0, &data);
		memcpy(data, m_Vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(device.device(), stagingBufferMemory.memory);

		createBuffer(bufferSize, device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

		copyBuffer(device, stagingBuffer, m_VertexBuffer, bufferSize);

		vkDestroyBuffer(device.device(), stagingBuffer, GetCallback());
		m_MemoryAllocator.FreeBuffer(stagingBufferMemory, stagingBuffer);
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
		m_MemoryAllocator.FreeBuffer(m_VertexBufferMemory, m_VertexBuffer);
	}

	void VulkanVertexBuffer::bindBuffer(VkCommandBuffer& commandBuffer)
	{
		VkBuffer vertexBuffers[] = { m_VertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	}

	void VulkanVertexBuffer::createBuffer(VkDeviceSize size, Device& device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, Allocation& bufferMemory)
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

		bufferMemory = m_MemoryAllocator.AllocateBuffer(size, buffer, usage, properties);

		vkBindBufferMemory(device.device(), buffer, bufferMemory.memory, bufferMemory.offset);
	}

}