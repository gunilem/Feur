#include "fpch.h"
#include "VulkanAllocator.h"


static MyAllocatorState* myState;
static VkAllocationCallbacks* myAllocator;

void* VKAPI_PTR MyAlloc(
	void* pUserData,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope)
{
	auto* state = static_cast<MyAllocatorState*>(pUserData);
	state->totalAllocated ++;

	void* ptr = malloc(size);//_aligned_malloc(size, alignment);
	memset(ptr, 0, size);
	return ptr;
}

void* VKAPI_PTR MyRealloc(
	void* pUserData,
	void* pOriginal,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope scope)
{
	return realloc(pOriginal, size);
}

void VKAPI_PTR MyFree(
	void* pUserData,
	void* pMemory)
{
	if (!pMemory) return;

	auto* state = static_cast<MyAllocatorState*>(pUserData);
	state->totalAllocated--;
	std::printf("[MyFree] Total: %zu\n", state->totalAllocated);
	free(pMemory);
}

void VKAPI_PTR MyInternalAllocNotify(
	void*,
	size_t size,
	VkInternalAllocationType,
	VkSystemAllocationScope)
{
	std::printf("[MyInternalAllocNotify] Internal alloc of %zu bytes\n", size);
}

void VKAPI_PTR MyInternalFreeNotify(
	void*,
	size_t size,
	VkInternalAllocationType,
	VkSystemAllocationScope)
{
	std::printf("[MyInternalFreeNotify] Internal free of %zu bytes\n", size);
}

void SetupAllocator() {

	myState = new MyAllocatorState();
	myAllocator = new VkAllocationCallbacks();

	myAllocator->pUserData = myState;
	myAllocator->pfnAllocation = MyAlloc;
	myAllocator->pfnReallocation = MyRealloc;
	myAllocator->pfnFree = MyFree;
	myAllocator->pfnInternalAllocation = MyInternalAllocNotify;
	myAllocator->pfnInternalFree = MyInternalFreeNotify;
}


void CleanAllocator() {
	delete myAllocator;
	delete myState;
}

VkAllocationCallbacks* GetCallback()
{
	return myAllocator;
}

MyAllocatorState* GetState()
{
	return myState;
}
