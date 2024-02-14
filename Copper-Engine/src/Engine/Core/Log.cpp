#include "cupch.h"
#include "Log.h"

#include "Engine/Core/Engine.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Copper {

	std::shared_ptr<spdlog::logger> Logger::m_logger;

	std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> Logger::m_ringbufferSink;
	std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> colorSink;

	// TODO: Add a log file like every other fucking app in the world

	void Logger::Initialize() {

		CUP_FUNCTION_START();

		// Can't use the macro since it uses the logger which is not initialized yet, duh

		if (EngineCore::GetEngineState() != EngineCore::EngineState::Entry) return;

		m_ringbufferSink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(100);
		colorSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

		m_ringbufferSink->set_pattern("%l:%v");
		colorSink->set_pattern("%^[%T] %n: %v%$");

		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(m_ringbufferSink);
		sinks.push_back(colorSink);

		m_logger = std::make_shared<spdlog::logger>("Logger", sinks.begin(), sinks.end());
		m_logger->set_level(spdlog::level::trace);

		CUP_SCOPE_END();

	}

}