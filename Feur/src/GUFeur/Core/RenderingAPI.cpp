#include "fpch.h"
#include "RenderingAPI.h"

#include "GUFeur/Renderer/Vulkan/VulkanRenderingAPI.h"

namespace GUFeur {
	RenderingAPI* RenderingAPI::create()
	{
		return new VulkanRenderingAPI();
	}
}