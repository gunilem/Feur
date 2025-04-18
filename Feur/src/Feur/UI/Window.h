#pragma once

#include "Widget.h"

namespace Feur {
	class Window : public Widget
	{
	public:
		Window(const std::string& title);

		virtual void draw() override;

	private:
		std::string m_Title;
	};
}

