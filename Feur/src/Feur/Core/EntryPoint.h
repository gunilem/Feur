#pragma once

#ifdef F_PLATFORM_WINDOWS

extern Feur::Application* Feur::CreateApplication();

int main(int args, char** argv) {

	Feur::Log::Init();
	F_CORE_WARN("Initialized Log!");
	int a = 5;
	F_INFO("Hello! Var={0}", a);

	auto app = Feur::CreateApplication();
	app->Run();

	delete app;
}
#else

#endif // F_PLATFORM_WINDOWS
