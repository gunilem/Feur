#pragma once

#include "GUFeur/Core/Core.h"

#include "GUFeur/Renderer/Vulkan/Device.h"

namespace GUFeur {

	struct Allocation {
		VkDeviceMemory memory;
		VkDeviceSize offset;
		VkDeviceSize size;
	};

	struct FreeRange {
		VkDeviceSize offset;
		VkDeviceSize size;
	};

	class MemoryBlock
	{
	public:
		MemoryBlock(Device& device);
		~MemoryBlock() {};

		GUF_DELETE_COPY_CONSTRUCTORS(MemoryBlock)



		void Init(uint32_t memoryTypeIndex, VkDeviceSize alignment, VkDeviceSize size);

		bool MemoryBlock::Allocate(VkDeviceSize allocSize, Allocation& outAlloc);
		bool MemoryBlock::Deallocate(const Allocation& outAlloc);

		void Free();

	private:
		Device& m_Device;
		VkDeviceMemory m_Memory = VK_NULL_HANDLE;
		VkDeviceSize m_Size = 0;

		std::vector<FreeRange> m_FreeList;
	};


	class VulkanMemoryAllocator {
	public:
		VulkanMemoryAllocator(Device& device);
		~VulkanMemoryAllocator() {};

		Allocation AllocateBuffer(VkDeviceSize size, VkBuffer buffer, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		void FreeBuffer(const Allocation& alloc, VkBuffer buffer); // Optional

		void FreeMemory();

	private:
		Device& m_Device;


		std::vector<MemoryBlock*> m_Blocks; // One per memory type
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	};
}

