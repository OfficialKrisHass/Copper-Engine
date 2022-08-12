#pragma once

#include <Copper.h>
#include <Engine/Core/Entry.h>

namespace Editor {

	void Initialize();

	void Run();
	void UI();

	void RenderDockspace();
	void RenderViewport();

}

void AppEntryPoint() {

	Editor::Initialize();

	Copper::SetEditorRunFunc(Editor::Run);
	Copper::SetEditorUIFunc(Editor::UI);

}