#pragma once

#include <Copper.h>

namespace Editor {

    class Project;

    class SceneMeta;

	enum EditorState {

		Edit, Play

	};

	void NewScene();
	void OpenScene();
	void OpenScene(const Copper::fs::path& path);
	void SaveScene();
	void SaveSceneAs();

	const Project& GetProject();

	SceneCamera& GetSceneCam();
	SceneMeta* GetSceneMeta();

	Copper::UVector2I GetViewportSize();

	void SetChanges(bool value);

}
