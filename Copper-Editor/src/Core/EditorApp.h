#pragma once

#include <Copper.h>

namespace Editor {

	void Initialize();

	void Run();
	void UI();

}

void AppEntryPoint() {

	Editor::Initialize();

	Copper::SetEditorRunFunc(Editor::Run);
	Copper::SetEditorUIFunc(Editor::UI);

}