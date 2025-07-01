#pragma once

#include <vulkan/vulkan.h>

void* VKAPI_PTR MyAlloc(
	void* pUserData,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope);

void* VKAPI_PTR MyRealloc(
	void* pUserData,
	void* pOriginal,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope);

void VKAPI_PTR MyFree(
	void* pUserData,
	void* pMemory);

void VKAPI_PTR MyInternalAllocNotify(
	void*,
	size_t size,
	VkInternalAllocationType,
	VkSystemAllocationScope);

void VKAPI_PTR MyInternalFreeNotify(
	void*,
	size_t size,
	VkInternalAllocationType,
	VkSystemAllocationScope);


struct MyAllocatorState {
	int64_t totalAllocated = 0;
};





void SetupAllocator();
void CleanAllocator();
VkAllocationCallbacks* GetCallback();
MyAllocatorState* GetState();