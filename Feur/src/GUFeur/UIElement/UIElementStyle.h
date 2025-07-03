#pragma once

namespace GUFeur {


	enum StyleFloatType {
		None = 0,
		Percentage = 1,
		Pixel = 2
	};

	struct StyleFloat {
		StyleFloatType Type;
		float Value = 0;


		StyleFloat& operator=(int value) {
			Type = StyleFloatType::Pixel;
			Value = value;
			return *this;
		}


		StyleFloat& operator=(std::string value) {
			if (value[value.size() - 1] != '%') return *this;

			Type = StyleFloatType::Percentage;
			for (int i = 0, y = value.size() - 2; i < value.size() - 2; i++, y--) {
				Value += value[i] * y;
			}

			std::cout << Value;
			return *this;
		}
	};

	struct Color3 {
		float r;
		float g;
		float b;

		Color3()
			: r(0), g(0), b(0) {
		}

		Color3(float red, float green, float blue)
			: r(red), g(green), b(blue) {
		}
	};


	struct UIElementStyle {
		StyleFloat Width, Height;
		Color3 BackgroundColor;
	};



}