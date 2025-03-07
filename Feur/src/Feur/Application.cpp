#include "fpch.h"
#include "Application.h"

#include "Feur/Events/ApplicationEvent.h"
#include "Feur/Log.h"

namespace Feur {
	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {

		WindowResizeEvent e(1280, 720);
		F_TRACE(e.ToString());

		while (true);
	}
};