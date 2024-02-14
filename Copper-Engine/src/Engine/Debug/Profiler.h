#pragma once

namespace Copper::Profiler {

	void StartScope(const char* name, const char* file);
	void EndScope();

	void PrintScopeStack();

}

#ifdef CU_DEBUG
#define CUP_SCOPE_START(name) ::Copper::Profiler::StartScope(name, __FILE__)
#define CUP_FUNCTION_START() CUP_SCOPE_START(__FUNCTION__)

#define CUP_SCOPE_END() ::Copper::Profiler::EndScope()
#elif
#define CUP_SCOPE_START(name)
#define CUP_FUNCTION_START()

#define CUP_END_SCOPE()
#endif