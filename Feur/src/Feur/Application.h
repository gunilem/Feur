#pragma once

#include "Core.h"

namespace Feur {

	class FEUR_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in the client
	Application* CreateApplication();
}