#pragma once

#ifdef F_PLATFORM_WINDOWS

extern Feur::Application* Feur::CreateApplication();

int main(int args, char** argv) {
	printf("Feur engine starting!");
	auto app = Feur::CreateApplication();
	app->Run();

	delete app;
}
#else

#endif // F_PLATFORM_WINDOWS
