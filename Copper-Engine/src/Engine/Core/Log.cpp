#include "cupch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ansicolor_sink.h>

namespace Copper {

	std::shared_ptr<spdlog::logger> Logger::logger;
	std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> Logger::ringbufferSink;
	std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> colorSink;

	void Logger::Initialize() {

		ringbufferSink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(100);
		colorSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

		ringbufferSink->set_pattern("%l:%v");
		colorSink->set_pattern("%^[%T] %n: %v%$");

		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(colorSink);
		sinks.push_back(ringbufferSink);

		logger = std::make_shared<spdlog::logger>("Logger", sinks.begin(), sinks.end());
		logger->set_level(spdlog::level::trace);

	}

}