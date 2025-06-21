#pragma once
#include "GUFeur/Renderer/Core/VertexBuffer.h"
#include "GUFeur/Renderer/Vulkan/Device.h"
#include <vulkan/vulkan.h>

#include "VulkanMemoryAllocator.h"

#include "VulkanAllocator.h"

namespace GUFeur {
	template<typename T>
	class VulkanBuffer : public Buffer<T>
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
		VulkanBuffer(BufferTypes type, std::vector<T>& data, VulkanMemoryAllocator& memoryAllocation);
		~VulkanBuffer() {};

		virtual void InitBuffer(Device& device);
		virtual void copyBuffer(Device& device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		virtual void cleanBuffer(Device& device);
		virtual void bindBuffer(VkCommandBuffer& commandBuffer);

	private:
		void createBuffer(VkDeviceSize size, Device& device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, Allocation& bufferMemory);


	private:
		VkBuffer m_Buffer; 
		Allocation m_BufferAllocation;

		VulkanMemoryAllocator& m_MemoryAllocator;

		VkMemoryPropertyFlags m_BufferTypeFlag;
	};

	template<typename T>
	inline VulkanBuffer<T>::VulkanBuffer(BufferTypes type, std::vector<T>& data, VulkanMemoryAllocator& memoryAllocation)
		: m_MemoryAllocator(memoryAllocation), Buffer<T>(type, data)
	{
		switch (type) {
		case BufferTypes::Vertex:
			m_BufferTypeFlag = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			break;
		case BufferTypes::Index:
			m_BufferTypeFlag = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			break;
		case BufferTypes::None:
			throw std::exception("No type initialized for a Vulkan Buffer");
			break;
		}
	}

	template<typename T>
	inline void VulkanBuffer<T>::InitBuffer(Device& device)
	{
		VkDeviceSize bufferSize = sizeof(bufferData()[0]) * bufferDataCount();


		VkBuffer stagingBuffer;
		Allocation stagingBufferMemory;
		createBuffer(bufferSize, device, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device.device(), stagingBufferMemory.memory, 0, bufferSize, 0, &data);
		memcpy(data, bufferData().data(), (size_t)bufferSize);
		vkUnmapMemory(device.device(), stagingBufferMemory.memory);

		createBuffer(bufferSize, device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | m_BufferTypeFlag, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_BufferAllocation);

		copyBuffer(device, stagingBuffer, m_Buffer, bufferSize);

		vkDestroyBuffer(device.device(), stagingBuffer, GetCallback());
		m_MemoryAllocator.FreeBuffer(stagingBufferMemory, stagingBuffer);
	}

	template<typename T>
	inline void VulkanBuffer<T>::copyBuffer(Device& device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
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

	template<typename T>
	inline void VulkanBuffer<T>::cleanBuffer(Device& device)
	{
		vkDestroyBuffer(device.device(), m_Buffer, GetCallback());
		m_MemoryAllocator.FreeBuffer(m_BufferAllocation, m_Buffer);
	}

	template<typename T>
	inline void VulkanBuffer<T>::bindBuffer(VkCommandBuffer& commandBuffer)
	{
		switch (getType()) 
		{
		case BufferTypes::Vertex:
			{
				VkBuffer buffers[] = { m_Buffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
				break;
			}
			case BufferTypes::Index:

				vkCmdBindIndexBuffer(commandBuffer, m_Buffer, 0, VK_INDEX_TYPE_UINT16);
				break;
			case BufferTypes::None:
				throw std::exception("No type initialized for a Vulkan Buffer");
				break;
		}

	}

	template<typename T>
	inline void VulkanBuffer<T>::createBuffer(VkDeviceSize size, Device& device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, Allocation& bufferAllocation)
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

		bufferAllocation = m_MemoryAllocator.AllocateBuffer(size, buffer, usage, properties);

		vkBindBufferMemory(device.device(), buffer, bufferAllocation.memory, bufferAllocation.offset);
	}

}