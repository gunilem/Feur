#include "fpch.h"
#include "Window.h"

namespace Feur{
	Window::Window(const std::string& title) : m_Title(title) {}

	void Window::draw()
	{
		std::cout << "Drawing window: " + m_Title << std::endl;

		Widget::draw();
	}
}