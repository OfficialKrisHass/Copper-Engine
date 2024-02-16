#include "cupch.h"
#include "Profiler.h"

#include <iostream>

#ifdef CU_DEBUG
namespace Copper::Profiler {

	static const char* CopperEngine = "Copper-Engine";
	static constexpr uint32 CopperEngineLen = 13;

	static std::vector<Scope*> scopeStack;

	static const char* RemovePath(const char* file);

	Scope::Scope(const char* name, const char* file) {

		this->name = name;
		this->file = RemovePath(file);

		scopeStack.push_back(this);


	}
	Scope::~Scope() {

		scopeStack.pop_back();

	}

	const char* RemovePath(const char* file) {

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

		return &file[i + 1];

	}

	void PrintScopeStack() {

		for (const Scope* scope : scopeStack) {

			Log("{}: {}", scope->name, scope->file);

		}

	}

	// Frame Profiling

	Frame mainFrame;
	Frame* latestFrame = nullptr;

	void StartFrame(const char* name) {

		// Main Frame

		if (!name) {

			mainFrame.name = "Main";
			mainFrame.parentFrame = nullptr;

			mainFrame.start = chrono::now();
			mainFrame.duration = 0.0f;

			mainFrame.subframes.clear();

			latestFrame = &mainFrame;

			return;

		}

		// Subframes

		latestFrame->subframes.push_back(Frame());
		Frame* frame = &latestFrame->subframes.back();

		frame->name = name;
		frame->parentFrame = latestFrame;

		frame->start = chrono::now();

		latestFrame = frame;

	}
	void EndFrame() {

		typedef std::chrono::microseconds microSec;

		chrono::duration tmp = chrono::now() - latestFrame->start;
		latestFrame->duration = (double) std::chrono::duration_cast<microSec>(tmp).count() / 1000.0;

		latestFrame = latestFrame->parentFrame;

	}

}
#endif