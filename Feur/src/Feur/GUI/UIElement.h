#pragma once

#include "Feur/Core/Base.h"

namespace Feur {

	class UIElement {
	public:
		static Ref<UIElement> Create();

	public:
		void AddChild(Ref<UIElement> child);

		virtual void Render() {};
		virtual void HandleEvent() {};
		
	private:
		std::vector<Ref<UIElement>> m_Children;
	};

}