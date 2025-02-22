#pragma once

#ifdef F_PLATFORM_WINDOWS
	#ifdef F_BUILD_DLL
		#define FEUR_API __declspec(dllexport)
	#else
		#define FEUR_API __declspec(dllimport)
	#endif // F_BUILD_DLL
#else
	#error Feur only supports Windows!
#endif // F_PLATFORM_WINDOWS
