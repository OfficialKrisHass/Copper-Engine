#pragma once

#include <Copper.h>

#include "Core/SceneMeta.h"

#include "Projects/Project.h"

namespace Editor {

	enum EditorState {

		Edit, Play

	};

	void Initialize();
	void UIUpdate();
	void Shutdown();

	void NewScene();
	void OpenScene();
	void OpenScene(const Filesystem::Path& path);
	void SaveScene();
	void SaveSceneAs();

	Project GetProject();

	MetaFile::SceneMeta* GetSceneMeta();

	Copper::UVector2I GetViewportSize();

	void SetChanges(bool value);

}