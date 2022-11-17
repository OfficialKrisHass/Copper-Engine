#pragma once

#include <Copper.h>

namespace Editor {

	enum EditorState {

		Edit, Play

	};

	void Initialize();

	void Run();
	void UI();

	void RenderDockspace();
	void RenderViewport();
	void RenderMenu();

	void NewScene();
	void OpenScene();
	void OpenScene(std::filesystem::path path);
	void SaveScene();
	void SaveSceneAs();

	void ManualScene();

	void SetChanges(bool value);

}