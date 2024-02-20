#include "Profiler.h"

namespace Editor::Profiler {

	using namespace Copper;
	typedef Copper::Profiler::Frame Frame;

	static bool isOpen = false;

	void Subframe(const Copper::Profiler::Frame& frame, Copper::uint32 depth = 0);

	void Open() { isOpen = true; }
	void UIRender() {

		if (!isOpen) return;
		ImGui::Begin("Profiler", &isOpen);

	#ifdef CU_DEBUG
		const Frame& frame = Copper::Profiler::PreviousMainFrame();

		for (const Frame& subframe : frame.subframes)
			Subframe(subframe);

		ImGui::Text("");
		ImGui::Text(fmt::format("Total: {}ms", frame.duration).c_str());
	#elif
		ImGui::Text("Profiler works only in Debug mode!");
	#endif

		ImGui::End();

	}

	void Subframe(const Frame& frame, uint32 depth) {

		std::string text = "\t";
		for (uint32 i = 0; i < depth; i++)
			text += '\t';

		text += fmt::format("Subframe: {} took {}ms", frame.name, frame.duration);
		ImGui::Text(text.c_str());

		for (const Frame& subframe : frame.subframes)
			Subframe(subframe, depth + 1);

	}

}