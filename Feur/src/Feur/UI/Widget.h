#pragma once

#include "Feur/Core/Device.h"

namespace Feur {

	struct WidgetVertices {
		float Position[2];
		float Color[3];
	};

	struct WidgetTransform {
		float X, Y;
		float Width, Height;
	};

	class Widget
	{
	public:
		Widget(Device& device, WidgetTransform& transform);
		~Widget();

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		void createVertextBuffers(const std::vector<WidgetVertices>& vertices);

	private:
		WidgetTransform m_Transform;
		std::vector<WidgetVertices> m_WidgetVertices;

		Device& m_Device;
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;
		uint32_t m_VertexCount;
	};

}
