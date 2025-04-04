#include "fpch.h"
#include "UIElement.h"

namespace Feur {
	Ref<UIElement> UIElement::Create()
	{
		return Ref<UIElement>();
	}

	void UIElement::AddChild(Ref<UIElement> child)
	{
		m_Children.push_back(child);
	}
}