#pragma once

#include <Copper.h>

#include "Core/Project.h"

namespace Editor {

	enum EditorState {

		Edit, Play

	};

	void Initialize();
	void UIUpdate();
	void Shutdown();

	void NewScene();
	void OpenScene();
	void OpenScene(const std::filesystem::path& path);
	void SaveScene();
	void SaveSceneAs();

	Project GetProject();
	Copper::UVector2I GetViewportSize();

	void SetChanges(bool value);

}