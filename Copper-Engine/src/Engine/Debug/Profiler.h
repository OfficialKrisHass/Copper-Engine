#pragma once

#include <chrono>

using chrono = std::chrono::system_clock;

namespace Copper::Profiler {

	// Code Scopes

	struct Scope {

		Scope(const char* name, const char* file);
		~Scope();

		const char* name = nullptr;
		const char* file = nullptr;

	};

	void CrashHandler(int sig);
	void PrintScopeStack();

	// Frame profiling

	struct Frame {

		const char* name = nullptr;
		Frame* parentFrame = nullptr;

		chrono::time_point start;
		double duration = 0.0f;

		std::vector<Frame> subframes;

	};

	void StartFrame(const char* name);
	void EndFrame();

	const Frame& PreviousMainFrame();

}

#ifdef CU_DEBUG
// THank the c++ compiler for being wonky that we have to pass the line macro through 2 macro functions in order
// to ## it :)))))))
#define __CUP_SCOPE(name, line) ::Copper::Profiler::Scope CU_SCOPE_##line = ::Copper::Profiler::Scope(name, __FILE__)

#define CUP_SCOPE(name, line) __CUP_SCOPE(name, line)
#define CUP_FUNCTION() CUP_SCOPE(__FUNCTION__, __LINE__)

#define CUP_START_FRAME(name) ::Copper::Profiler::StartFrame(name)
#define CUP_END_FRAME() ::Copper::Profiler::EndFrame()
#else
#define CUP_SCOPE(name, line)
#define CUP_FUNCTION()

#define CUP_START_FRAME(name)
#define CUP_END_FRAME()
#endif