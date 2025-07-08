#include "fpch.h"
#include "TabLayout.h"



namespace GUFeur {
	TabLayout::TabLayout()
	{
		style.Width = { 1.0f, Percentage1 };
		style.Height = { 1.0f, Percentage1 };

		UIElement* headerComponent = new UIElement();
		headerComponent->style.Height = { 30.f, Pixel };
		headerComponent->style.BackgroundColor = { 0.1f, 0.1f, 0.1f };

		UIElement* bodyComponent = new UIElement();
		bodyComponent->style.Height = { 1.0f, Percentage1 };
		bodyComponent->style.BackgroundColor = { 0.5f, 0.5f, 0.5f };

		addChild(headerComponent);
		addChild(bodyComponent);
	}

	TabLayout::~TabLayout()
	{
	}
}