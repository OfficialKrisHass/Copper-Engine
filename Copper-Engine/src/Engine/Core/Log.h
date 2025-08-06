#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <spdlog/sinks/ringbuffer_sink.h>

#if defined(CU_DEBUG) | defined(CU_EDITOR)

#define Log(...)        Copper::Logger::GetLogger()->trace(__VA_ARGS__)
#define LogInfo(...)    Copper::Logger::GetLogger()->info(__VA_ARGS__)
#define LogWarn(...)    Copper::Logger::GetLogger()->warn(__VA_ARGS__)
#define LogError(...)   Copper::Logger::GetLogger()->error(__VA_ARGS__)

#else

#define Log(...)
#define LogWarn(...)
#define LogTrace(...)
#define LogError(...)

#endif

#ifdef CU_DEBUG
#define LogDebug(...)   Copper::Logger::GetLogger()->debug(__VA_ARGS__)
#else
#define LogDebug(...)
#endif

namespace Copper {

    // TODO: maybe remove in favour of a custom console in Editor ?

    class Logger {

    public:
        typedef std::function<void(const spdlog::details::log_msg&)> CallbackFunc;

        static void Initialize(); // Internal, aka DO NOT USE

        inline static std::shared_ptr<spdlog::logger> GetLogger() { return m_logger; }

        inline static void SetCallback(const CallbackFunc& callback) { m_callback = callback; }

    private:
        static std::shared_ptr<spdlog::logger> m_logger;
        static CallbackFunc m_callback;

        static void LogCallback(const spdlog::details::log_msg& msg);

    };

}
