#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <spdlog/sinks/ringbuffer_sink.h>

#ifdef CU_DEBUG

#define Log(...)        Copper::Logger::GetLogger()->trace(__VA_ARGS__)
#define LogWarn(...)    Copper::Logger::GetLogger()->warn(__VA_ARGS__)
#define LogError(...)   Copper::Logger::GetLogger()->error(__VA_ARGS__)
#define LogSuccess(...) Copper::Logger::GetLogger()->info(__VA_ARGS__)

#else

#define Log(...)
#define LogWarn(...)
#define LogTrace(...)
#define LogError(...)

#endif

namespace Copper {

    // TODO: maybe remove in favour of a custom console in Editor ?

    class Logger {

    public:
        static void Initialize(); // Internal, aka DO NOT USE

        inline static std::shared_ptr<spdlog::logger> GetLogger() { return m_logger; }

    private:
        static std::shared_ptr<spdlog::logger> m_logger;

    };

}
