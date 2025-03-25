#include "SandboxApp.h"

#include <Feur/Core/EntryPoint.h>

using namespace Feur;

namespace Sandbox {
	
	//------Sandbox-------------
	Sandbox::Sandbox() {

	}

	Sandbox::~Sandbox() {

	}
	//-------------------------
}

Application* Feur::CreateApplication() {
	return new Sandbox::Sandbox();
}