#pragma once

#include "GUFeur/Core/RenderingAPI.h"
#include "UIElementStyle.h"

namespace GUFeur {

	struct UIData
	{
		float Width;
		float Height;

		float X, Y;

		UIData() {};

		UIData(float width, float height, float x, float y)
			: Width(width), Height(height), X(x), Y(y) {}
	};

	class UIElement
	{
	public:
		UIElement();
		~UIElement();

		virtual void update(RenderingAPI* renderer, UIData& parentData);
		virtual void draw(RenderingAPI* renderer);
		virtual void cleanup(RenderingAPI* renderer);

		void addChild(UIElement* child) { m_Children.push_back(child); }

	public:
		UIElementStyle style;


	private:
		Model* m_Model = nullptr;

		std::vector <Vertex> m_Vertices;
		std::vector<uint16_t> m_Indices;

		std::vector<UIElement*> m_Children;
	};
}

