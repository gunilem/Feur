#include "fpch.h"
#include "Widget.h"

namespace Feur {
	void Widget::draw()
	{
		for (auto child : m_Children) {
			child->draw();
		}
	}

	void Widget::update()
	{
		for (auto child : m_Children) {
			child->update();
		}
	}

	void Widget::onEvent()
	{
		for (auto child : m_Children) {
			child->onEvent();
		}
	}

	void Widget::addChild(std::shared_ptr<Widget> child) {
		child->m_Parent = this;
		m_Children.push_back(child);
	}
}