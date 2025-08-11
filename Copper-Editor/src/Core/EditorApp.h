#pragma once

#include <Copper.h>

namespace Editor {

    class Project;

    enum class EditorState {

        Edit, Play

    };

    void NewScene();
    void OpenScene(bool checkUnsavedChanges = true);
    void OpenScene(const Copper::fs::path& path, bool checkUnsavedChanges = true);
    void SaveScene();
    void SaveSceneAs();

    void OpenSceneNext(const Copper::fs::path& path);
    void OpenSceneNext();

    // Checks if there are unsaved changes, if so, prompts the user to either save or discard them.
    // Returns true if user selected cancel, aka, the operation should not be continued.
    bool EnsureUnsavedChanges();

    bool IsInteractionBlocked();
    void SetIsInteractionBlocked(bool value);

    const std::string& GetWindowTitle();

    Project& GetProject();

    SceneCamera& GetSceneCam();

    bool IsRuntimeRunning();

    Copper::UVector2I GetViewportSize();

    void SetWindowTitle(const std::string& title);

}
