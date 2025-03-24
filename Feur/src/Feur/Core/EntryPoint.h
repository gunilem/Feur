#pragma once

#ifdef F_PLATFORM_WINDOWS

extern Feur::Application* Feur::CreateApplication();

int main(int args, char** argv) {

	Feur::Log::Init();

	F_PROFILE_BEGIN_SESSION("Startup", "FeurProfile-Startup.json");
	auto app = Feur::CreateApplication();
	F_PROFILE_END_SESSION();

	F_PROFILE_BEGIN_SESSION("Runtime", "FeurProfile-Runtime.json");
	app->Run();
	F_PROFILE_END_SESSION();

	F_PROFILE_BEGIN_SESSION("Shutdown", "FeurProfile-Shutdown.json");
	delete app;
	F_PROFILE_END_SESSION();
}
#else

#endif // F_PLATFORM_WINDOWS
