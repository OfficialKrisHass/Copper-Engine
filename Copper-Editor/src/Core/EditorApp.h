#pragma once

#include <Copper.h>

namespace Editor {

	enum EditorState {

		Edit, Play

	};

	void Initialize();

	void Run();
	void UI();

	void Shutdown();

	void RenderDockspace();
	void RenderViewport();
	void RenderToolbar();
	void RenderMenu();

	void SaveEditorData();
	void LoadEditorData();
	void SaveProjectData();
	void LoadProjectData();

	void NewProject();
	void OpenProject();

	void NewScene();
	void OpenScene();
	void OpenScene(std::filesystem::path path);
	void SaveScene();
	void SaveSceneAs();

	bool OnKeyPressed(Copper::Event& e);
	bool OnWindowClose(Copper::Event& e);

	void ManualScene();

	std::filesystem::path GetProjectPath();

	void SetChanges(bool value);

}