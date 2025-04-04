#pragma once

#include "Feur/GUI/UIElement.h"
#include "Feur/Core/Base.h"


namespace Feur {

	struct WindowProperties {
		const char* Name;

		unsigned int X, Y;
		unsigned int Width, Height;
	};


	class Window {
	public:
		Window(const WindowProperties& windowProperty);
		virtual ~Window();

		inline virtual const WindowProperties& GetProperties() const { return m_Properties; }
		inline virtual Ref<UIElement> GetRoot() const { return m_Root; }

	private:
		WindowProperties m_Properties;
		Ref<UIElement> m_Root;
		void* m_NativeWindow;
	};

}