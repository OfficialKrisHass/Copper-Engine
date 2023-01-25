#pragma once

#include <Copper.h>

#include "Core/Project.h"

namespace Editor {

	enum EditorState {

		Edit, Play

	};

	void Initialize();

	void Run();
	void Shutdown();

	void UI();


	void RenderDockspace();
	void RenderGamePanel();
	void RenderViewport();
	void RenderToolbar();
	void RenderMenu();

	void NewScene();
	void OpenScene();
	void OpenScene(std::filesystem::path path);
	void SaveScene();
	void SaveSceneAs();

	bool OnKeyPressed(const Copper::Event& e);
	bool OnWindowClose(const Copper::Event& e);
	bool OnWindowFocused(const Copper::Event& e);

	Project GetProject();

	Copper::UVector2I GetViewportSize();

	void ManualScene();

	void SetChanges(bool value);

}