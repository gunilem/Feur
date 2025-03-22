#pragma once

#include "Feur/Core/Input.h"

namespace Feur {
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPresedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;

		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}