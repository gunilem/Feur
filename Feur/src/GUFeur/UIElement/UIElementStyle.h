#pragma once

namespace GUFeur {


	enum StyleFloatType {
		None = 0,
		Percentage100 = 1,
		Percentage1 = 2,
		Pixel = 3
	};

	struct StyleFloat {
		StyleFloatType Type;
		float Value = 0;


		StyleFloat()
			: Type{ StyleFloatType::Percentage1 }, Value{ 1.0f } {
		}

		StyleFloat(float value, StyleFloatType type)
			: Type{ type }, Value{value} { }


		StyleFloat(StyleFloatType type)
			: Type{ type } {
		}


		float getValue(float parentProperty) {
			switch (Type) {
			case StyleFloatType::Percentage100:
				return parentProperty * (Value / 100);
				break;
			case StyleFloatType::Percentage1:
				return parentProperty * Value;
				break;
			case StyleFloatType::Pixel:
				return Value;
				break;
			case StyleFloatType::None:
				return 0;
				break;
			}
			return 0;
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

		UIElementStyle() 
		: MarginLeft(None), MarginRight(None), MarginTop(None), MarginBottom(None) {

		}

		StyleFloat Width, Height;
		StyleFloat MarginLeft, MarginRight, MarginTop, MarginBottom;
		Color3 BackgroundColor;
	};



}