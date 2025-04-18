#pragma once

namespace Feur {
	class Widget
	{
	public:
		virtual void draw();
		virtual void update();
		virtual void onEvent();

		void addChild(std::shared_ptr<Widget> child);

		virtual ~Widget() {
			std::cout << "widget destroyed" << std::endl;
		}

	protected:
		Widget* m_Parent = nullptr;
		std::vector< std::shared_ptr<Widget>> m_Children;

	private:
		int x, y;
		uint32_t width, height;
	};
}
