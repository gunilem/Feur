#include "fpch.h"
#include "Widget.h"


namespace Feur {



	Widget::Widget(Device& device, WidgetTransform& transform)
		: m_Device(device), m_Transform(transform)
	{
		m_WidgetVertices = {
			{{  m_Transform.X, m_Transform.Y }, {0.8f, 0.2f, 0.3f}},
			{{  m_Transform.X + m_Transform.Width, m_Transform.Y }, {0.3f, 0.8f, 0.2f}},
			{{  m_Transform.X + m_Transform.Width, m_Transform.Y + m_Transform.Height }, {0.2f, 0.3f, 0.8f}},
			{{  m_Transform.X, m_Transform.Y + m_Transform.Height  }, {0.2f, 0.3f, 0.8f}}
		};
		createVertextBuffers(m_WidgetVertices);
	}

	Widget::~Widget()
	{
		vkDestroyBuffer(m_Device.device(), m_VertexBuffer, nullptr);
		vkFreeMemory(m_Device.device(), m_VertexBufferMemory, nullptr);
	}

	void Widget::Bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_VertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void Widget::Draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
	}



	void Widget::createVertextBuffers(const std::vector<WidgetVertices>& vertices)
	{
		m_VertexCount = static_cast<uint32_t>(vertices.size());
		VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount;
		m_Device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			m_VertexBuffer,
			m_VertexBufferMemory);

		void* data;
		vkMapMemory(m_Device.device(), m_VertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_Device.device(), m_VertexBufferMemory);
	}

}