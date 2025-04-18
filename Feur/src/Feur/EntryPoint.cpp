#include "fpch.h"

#include "Core/Application.h"

using namespace Feur;

int main() {
	Application* feur = Application::Create("Feur");
	feur->Init();
	feur->Run();
	delete feur;
}