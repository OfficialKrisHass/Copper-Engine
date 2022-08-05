#pragma once

#include "Engine/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Copper {

#ifdef CU_DEBUG

	#define Log(...)		Copper::Logger::GetLogger()->info(__VA_ARGS__)
	#define LogWarn(...)	Copper::Logger::GetLogger()->warn(__VA_ARGS__)
	#define LogTrace(...)	Copper::Logger::GetLogger()->trace(__VA_ARGS__)
	#define LogError(...)	Copper::Logger::GetLogger()->error(__VA_ARGS__)

#elif CU_RELEASE

	#define Log(...)
	#define LogWarn(...)
	#define LogTrace(...)
	#define LogError(...)

#endif

	class COPPER_API Logger {

	public:
		static void Initialize();

		inline static std::shared_ptr<spdlog::logger> GetLogger() { return logger; }

	private:
		static std::shared_ptr<spdlog::logger> logger;

	};

}