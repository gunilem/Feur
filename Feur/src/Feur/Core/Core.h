#pragma once

#include <memory>

#ifdef F_PLATFORM_WINDOWS
	#if F_DYNAMIC_LINK
		#ifdef F_BUILD_DLL
			#define FEUR_API __declspec(dllexport)
		#else
			#define FEUR_API __declspec(dllimport)
		#endif // F_BUILD_DLL
	#else
		#define FEUR_API
	#endif
#else
	#error Feur only supports Windows!
#endif // F_PLATFORM_WINDOWS

#ifdef F_DEBUG
	#define F_ENABLE_ASSERTS
#endif // 

#ifdef F_ENABLE_ASSERTS
	#define F_ASSERT(x, ...) { if(!(x)) { F_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define F_CORE_ASSERT(x, ...) { if(!(x)) { F_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define F_ASSERT(x, ...) 
	#define F_CORE_ASSERT(x, ...) 
#endif // F_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Feur {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}