#include "fpch.h"
#include "UIElement.h"

#include "GUFeur/Renderer/Core/Model.h"

namespace GUFeur {
	UIElement::UIElement(UIData& parentData)
		: m_ParentData(parentData), style{}
	{
		style.Width = "100%";
		style.Height = "100%";

		m_Indices = {
			0, 1, 2, 2, 3, 0
		};
	}

	UIElement::~UIElement()
	{
	}

	void UIElement::update(RenderingAPI* renderer)
	{
		m_Vertices = {
			{{0.0f, 0.0f}, style.BackgroundColor},
			{{m_ParentData.Width , 0.0f}, style.BackgroundColor},
			{{m_ParentData.Width , m_ParentData.Height}, style.BackgroundColor},
			{{0.0f, m_ParentData.Height}, style.BackgroundColor}
		};

		style.BackgroundColor = {0.1f, 0.1f, 0.1f};

		if (m_Model == nullptr) {
			m_Model = new Model{ *renderer, m_Vertices , m_Indices };
		}
		else {
			renderer->updateVertexBufferData(m_Model->VertexBuffer);
		}
	}

	void UIElement::draw(RenderingAPI* renderer)
	{
		renderer->drawModel(*m_Model);
	}

	void UIElement::cleanup(RenderingAPI* renderer)
	{
		m_Model->cleanup(*renderer);
		delete m_Model;
	}
}