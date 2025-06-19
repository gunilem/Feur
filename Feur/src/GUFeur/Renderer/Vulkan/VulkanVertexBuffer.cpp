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

	void VulkanVertexBuffer::createBuffer(Device& device)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeof(m_Vertices[0]) * m_VertexCount;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device.device(), &bufferInfo, GetCallback(), &m_VertexBuffer) != VK_SUCCESS) {
			throw std::runtime_error("echec de la creation d'un vertex buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.device(), m_VertexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		if (vkAllocateMemory(device.device(), &allocInfo, nullptr, &m_VertexBufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("echec d'une allocation de memoire!");
		}

		vkBindBufferMemory(device.device(), m_VertexBuffer, m_VertexBufferMemory, 0);

		void* data;
		vkMapMemory(device.device(), m_VertexBufferMemory, 0, bufferInfo.size, 0, &data);
		memcpy(data, m_Vertices.data(), (size_t)bufferInfo.size);
		vkUnmapMemory(device.device(), m_VertexBufferMemory);
	}

	void VulkanVertexBuffer::cleanBuffer(Device& device)
	{
		vkDestroyBuffer(device.device(), m_VertexBuffer, GetCallback());
		vkFreeMemory(device.device(), m_VertexBufferMemory, nullptr);
	}

	void VulkanVertexBuffer::bindBuffer(VkCommandBuffer& commandBuffer)
	{
		VkBuffer vertexBuffers[] = { m_VertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	}

}