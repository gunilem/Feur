#include "fpch.h"
#include "Button.h"

namespace Feur 
{
	Button::Button(const std::string& label) : m_Label(label) {}

	void Button::draw()
	{
		std::cout << "Drawing Button: " + m_Label << std::endl;

		Widget::draw();
	}
}