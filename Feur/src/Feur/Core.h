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

#ifdef F_ENABLE_ASSERTS
	#define F_ASSERT(x, ...) { if(!(x)) { F_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define F_CORE_ASSERT(x, ...) { if(!(x)) { F_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define F_ASSERT(x, ...) 
	#define F_CORE_ASSERT(x, ...) 
#endif // F_ENABLE_ASSERTS


#define BIT(x) (1 << x)