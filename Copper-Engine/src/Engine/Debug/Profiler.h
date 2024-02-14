#pragma once

namespace Copper::Profiler {

	class Scope {

	public:
		Scope(const char* name, const char* file);
		~Scope();

		const char* name = nullptr;
		const char* file = nullptr;

	};

	void PrintScopeStack();

}

#ifdef CU_DEBUG
// THank the c++ compiler for being wonky that we have to pass the line macro through 2 macro functions in order
// to ## it :)))))))
#define __CUP_SCOPE(name, line) ::Copper::Profiler::Scope CU_SCOPE_##line = ::Copper::Profiler::Scope(name, __FILE__);

#define CUP_SCOPE(name, line) __CUP_SCOPE(name, line)
#define CUP_FUNCTION() CUP_SCOPE(__FUNCTION__, __LINE__)
#elif
#define CUP_SCOPE(name, line)
#define CUP_FUNCTION()
#endif