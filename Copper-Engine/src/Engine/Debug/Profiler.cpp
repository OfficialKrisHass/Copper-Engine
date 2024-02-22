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

	void CrashHandler(int32 sig) {

		LogError("A crash ({}) has occured in the Copper-Engine!\nCall stack:\n", sig);
		PrintScopeStack();

	}
	void PrintScopeStack() {

		for (const Scope* scope : scopeStack) {

			Log("{}: {}", scope->name, scope->file);

		}

	}

	// Frame Profiling

	Frame mainFrameBuffer[2];
	bool mainFramIndex = 0;

	Frame* latestFrame = nullptr;

	Frame& MainFrame() { return mainFrameBuffer[mainFramIndex]; }
	const Frame& PreviousMainFrame() { return mainFrameBuffer[!mainFramIndex]; }

	void StartFrame(const char* name) {

		// Main Frame

		if (!name) {

			MainFrame().name = "Main";
			MainFrame().parentFrame = nullptr;

			MainFrame().start = chrono::now();
			MainFrame().duration = 0.0f;

			MainFrame().subframes.clear();

			latestFrame = &MainFrame();

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

		typedef std::chrono::milliseconds microSecs;

		chrono::duration tmp = chrono::now() - latestFrame->start;
		latestFrame->duration = (double) std::chrono::duration_cast<microSecs>(tmp).count();

		latestFrame = latestFrame->parentFrame;

		if (latestFrame) return;
		mainFramIndex = !mainFramIndex;

	}

}
#endif