#include "fpch.h"
#include "VulkanMemoryAllocator.h"

#include "VulkanAllocator.h"

namespace GUFeur {
	MemoryBlock::MemoryBlock(Device& device)
		: m_Device(device)
	{

	}

	void MemoryBlock::Init(uint32_t memoryTypeIndex, VkDeviceSize alignment, VkDeviceSize size)
	{
		VkMemoryAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		allocInfo.allocationSize = std::max(size * 8, (VkDeviceSize)(64 * 1024)); // 64KB
		allocInfo.memoryTypeIndex = memoryTypeIndex;

		m_Size = allocInfo.allocationSize;

		if (vkAllocateMemory(m_Device.device(), &allocInfo, GetCallback(), &m_Memory) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate memory.");

		m_FreeList.resize((64 * 1024) / (size + alignment));

		uint32_t currentOffset = 0;

		for (auto& freeRange : m_FreeList) {
			uint32_t align = alignment - (size % alignment);
			currentOffset += size + (align % alignment);

			if (size + alignment > m_Size)
				throw std::runtime_error("free List too big");

			freeRange.offset = currentOffset;
			freeRange.size = size + align;
		}
	}

	bool MemoryBlock::Allocate(VkDeviceSize allocSize, Allocation& outAlloc) {

		for (int i = 0; i < m_FreeList.size(); i++) {
			if (allocSize > m_FreeList[i].size) {
				throw std::runtime_error("allocation too big for the memory layout");
				return false;
			}
			outAlloc.memory = m_Memory;
			outAlloc.offset = m_FreeList[i].offset;
			outAlloc.size = m_FreeList[i].size;

			m_FreeList.erase(m_FreeList.begin() + i);
			return true;
		}

		return false;
	}

	bool MemoryBlock::Deallocate(const Allocation& outAlloc) {
		if (outAlloc.memory != m_Memory) return false;

		FreeRange range{};
		range.offset = outAlloc.offset;
		range.size = outAlloc.size;

		m_FreeList.push_back(std::move(range));

		return true;
	}

	void MemoryBlock::Free()
	{
		vkFreeMemory(m_Device.device(), m_Memory, GetCallback());
	}





	VulkanMemoryAllocator::VulkanMemoryAllocator(Device& device)
		: m_Device(device)
	{

	}

	Allocation VulkanMemoryAllocator::AllocateBuffer(VkDeviceSize size, VkBuffer buffer, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		Allocation resultAllocation;

		VkMemoryRequirements memoryRequierement;
		vkGetBufferMemoryRequirements(m_Device.device(), buffer, &memoryRequierement);

		uint32_t memoryTypeIndex = FindMemoryType(memoryRequierement.memoryTypeBits, properties);

		// Step 2: Search existing pool
		for (auto& block : m_Blocks) {
			if (block && block->Allocate(memoryRequierement.size, resultAllocation)) {
				return resultAllocation;
			}
		}

		// Step 3: No pool found — allocate a new big block

		auto newBlock = new MemoryBlock(m_Device);
		newBlock->Init(memoryTypeIndex, memoryRequierement.alignment, memoryRequierement.size);

		if (!newBlock->Allocate(memoryRequierement.size, resultAllocation))
			throw std::runtime_error("New block couldn't satisfy allocation!");

		m_Blocks.push_back({ newBlock });

		return resultAllocation;
	}

	void VulkanMemoryAllocator::FreeBuffer(const Allocation& alloc, VkBuffer buffer)
	{
		for (int i = m_Blocks.size() - 1; i >= 0; i--)
		{
			if (m_Blocks[i]->Deallocate(alloc)) {
				vkDestroyBuffer(m_Device.device(), buffer, GetCallback());
				return;
			}
		}
		throw std::runtime_error("failed to free the buffer");
	}

	void VulkanMemoryAllocator::FreeMemory()
	{
		for (auto block : m_Blocks) {
			block->Free();
			delete block;
		}
	}

	uint32_t VulkanMemoryAllocator::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProps;
		vkGetPhysicalDeviceMemoryProperties(m_Device.physicalDevice(), &memProps);

		for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProps.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}

		throw std::runtime_error("Failed to find suitable memory type.");
	}
}