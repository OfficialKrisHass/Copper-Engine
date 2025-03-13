#pragma once

#include <Copper.h>

namespace Editor {

    class Project;

    struct SelectedAsset;

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

    Copper::Entity& GetSelectedEntity();
    Copper::fs::path& GetSelectedFile();

    void SetSelectedEntity(const Copper::Entity& value);
    void SetSelectedFile(const Copper::fs::path& value);

    bool IsRuntimeRunning();

    Copper::UVector2I GetViewportSize();

    void SetChanges(bool value);

}
