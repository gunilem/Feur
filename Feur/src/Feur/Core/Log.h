#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Feur {

	class FEUR_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core log
//#define F_CORE_FATAL(...)     ::Feur::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define F_CORE_ERROR(...)     ::Feur::Log::GetCoreLogger()->error(__VA_ARGS__)
#define F_CORE_WARN(...)      ::Feur::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define F_CORE_INFO(...)      ::Feur::Log::GetCoreLogger()->info(__VA_ARGS__)
#define F_CORE_TRACE(...)     ::Feur::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client log
//#define F_FATAL(...)          ::Feur::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define F_ERROR(...)          ::Feur::Log::GetClientLogger()->error(__VA_ARGS__)
#define F_WARN(...)           ::Feur::Log::GetClientLogger()->warn(__VA_ARGS__)
#define F_INFO(...)           ::Feur::Log::GetClientLogger()->info(__VA_ARGS__)
#define F_TRACE(...)          ::Feur::Log::GetClientLogger()->trace(__VA_ARGS__) 