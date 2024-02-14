#include "cupch.h"
#include "Profiler.h"

#include <chrono>

#ifdef CU_DEBUG
namespace Copper::Profiler {

	static const char* CopperEngine = "Copper-Engine";
	static constexpr uint32 CopperEngineLen = 13;

	struct Scope {

		Scope() = default;
		Scope(const char* name, const char* file) : name(name) {

			uint32 i = 0;
			uint32 j = 0;
			while (file[i]) {

				if (file[i] != CopperEngine[j]) {
					
					j = 0;
					i++;

					continue;
					
				}

				j++;
				i++;

				if (j == CopperEngineLen) break;

			}

			this->file = &file[i + 1];

		}

		const char* name = nullptr;
		const char* file = nullptr;

	};

	std::vector<Scope> scopeStack;

	void StartScope(const char* name, const char* file) {

		scopeStack.push_back(Scope(name, file));

	}
	void EndScope() {

		scopeStack.pop_back();

	}

	void PrintScopeStack() {

		for (const Scope& scope : scopeStack) {

			Log("{}: {}", scope.name, scope.file);

		}

	}

}
#endif