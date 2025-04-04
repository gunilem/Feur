#pragma once

#include "Feur/Core/Application.h"

extern Feur::Application* CreateApplication();

int main(int args, char** argv) {
	auto app = CreateApplication();
	app->Run();
}