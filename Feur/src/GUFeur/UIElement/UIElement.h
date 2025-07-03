#pragma once

#include "GUFeur/Core/RenderingAPI.h"
#include "UIElementStyle.h"

namespace GUFeur {

	struct UIData
	{
		float Width;
		float Height;

		UIData(float width, float height)
			: Width(width), Height(height) {}
	};

	class UIElement
	{
	public:
		UIElement(UIData& parentData);
		~UIElement();

		virtual void update(RenderingAPI* renderer);
		virtual void draw(RenderingAPI* renderer);
		virtual void cleanup(RenderingAPI* renderer);

	private:
		Model* m_Model = nullptr;

		UIData& m_ParentData;
		UIElementStyle style;

		std::vector < Vertex > m_Vertices;
		std::vector<uint16_t> m_Indices;
	};
}

