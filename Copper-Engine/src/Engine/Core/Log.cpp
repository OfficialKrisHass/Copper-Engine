#include "cupch.h"
#include "Log.h"

#include "Engine/Core/Engine.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Copper {

    static bool initialized = false;

    std::shared_ptr<spdlog::logger> Logger::m_logger;

    static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> colorSink;

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

        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(colorSink);

        m_logger = std::make_shared<spdlog::logger>("Logger", sinks.begin(), sinks.end());
        m_logger->set_level(spdlog::level::trace);

    }

}
