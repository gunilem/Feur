#pragma once

#include "Widget.h"

namespace Feur {
	class Button : public Widget
	{
	public:
		Button(const std::string& label);

		virtual void draw() override;

	private:
		std::string m_Label;
	};
}

