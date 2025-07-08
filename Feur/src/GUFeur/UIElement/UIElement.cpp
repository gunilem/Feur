#include "fpch.h"
#include "UIElement.h"

#include "GUFeur/Renderer/Core/Model.h"

namespace GUFeur {
	UIElement::UIElement()
		: style{ }
	{

		m_Indices = {
			0, 1, 2, 2, 3, 0
		};
	}

	UIElement::~UIElement()
	{
	}

	void UIElement::update(RenderingAPI* renderer, UIData& parentData)
	{
		float width = style.Width.getValue(parentData.Width);
		float height = style.Height.getValue(parentData.Height);

		float posX = parentData.X + style.MarginLeft.getValue(parentData.Width);
		float posY = parentData.Y + style.MarginTop.getValue(parentData.Height);

		UIData data{ width, height, posX, posY };

		m_Vertices = {
			{{posX, posY}, style.BackgroundColor},
			{{posX + data.Width, posY}, style.BackgroundColor},
			{{posX + data.Width, posY + data.Height}, style.BackgroundColor},
			{{posX, posY + data.Height}, style.BackgroundColor}
		};

		if (m_Model == nullptr) {
			m_Model = new Model{ *renderer, m_Vertices , m_Indices };
		}
		else {
			renderer->updateVertexBufferData(m_Model->VertexBuffer);
		}

		for (UIElement* child : m_Children) {
			child->update(renderer, data);
		}

		parentData.Y += height + style.MarginBottom.getValue(parentData.Height);
	}

	void UIElement::draw(RenderingAPI* renderer)
	{
		renderer->drawModel(*m_Model);
		for (UIElement* child : m_Children) {
			child->draw(renderer);
		}
	}

	void UIElement::cleanup(RenderingAPI* renderer)
	{
		for (UIElement* child : m_Children) {
			child->cleanup(renderer);
			delete child;
		}

		m_Model->cleanup(*renderer);
		delete m_Model;
	}
}