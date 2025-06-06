#include "cupch.h"
#include "Log.h"

#include "Engine/Core/Engine.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/callback_sink.h>

namespace Copper {

    static bool initialized = false;

    std::shared_ptr<spdlog::logger> Logger::m_logger;
    Logger::CallbackFunc Logger::m_callback = nullptr;

    static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> colorSink;
    static std::shared_ptr<spdlog::sinks::callback_sink_mt> callbackSink;

    // TODO: Add a log file like every other fucking app in the world

    void Logger::Initialize() {

        CUP_FUNCTION();

        if (initialized) {

            LogError("Can not initialize the logger as it has already been initialized.");
            return;

        }
        initialized = true;

        colorSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        colorSink->set_pattern("%^[%T] %n: %v%$");

        callbackSink = std::make_shared<spdlog::sinks::callback_sink_mt>(Logger::LogCallback);

        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(colorSink);;
        sinks.push_back(callbackSink);

        m_logger = std::make_shared<spdlog::logger>("Logger", sinks.begin(), sinks.end());
        m_logger->set_level(spdlog::level::trace);

    }

    void Logger::LogCallback(const spdlog::details::log_msg& msg) {

        CUP_FUNCTION();

        if (!m_callback) return;
        m_callback(msg);

    }

}
