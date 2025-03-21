#pragma once

#include <Copper.h>

namespace Editor {

    class Project;

    enum class EditorState {

        Edit, Play

    };

    void NewScene();
    void OpenScene();
    void OpenScene(const Copper::fs::path& path);
    void SaveScene();
    void SaveSceneAs();

    void OpenSceneNext(const Copper::fs::path& path);
    void OpenSceneNext();

    const Project& GetProject();

    SceneCamera& GetSceneCam();

    bool IsRuntimeRunning();

    Copper::UVector2I GetViewportSize();

    void SetChanges();

}
