#pragma once

#include <Copper.h>

#include "Core/SceneMeta.h"

#include "Projects/Project.h"

namespace Editor {

	enum EditorState {

		Edit, Play

	};

	void NewScene();
	void OpenScene();
	void OpenScene(const fs::path& path);
	void SaveScene();
	void SaveSceneAs();

	const Project& GetProject();
	SceneCamera& GetSceneCam();
	SceneMeta* GetSceneMeta();
	Copper::UVector2I GetViewportSize();

	void SetChanges(bool value);

}