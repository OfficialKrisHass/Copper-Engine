#include "ChangeHandler.h"

#include "Core/EditorApp.h"

namespace Editor {

    std::vector<Change> changeStack;

    bool unsaved = false;

    Change& AddChange(Change::Type type) {

        CUP_FUNCTION();

        if (!unsaved) {

            unsaved = true;

            std::string title = GetWindowTitle(); 
            title += '*';
            Input::SetWindowTitle(title);

        }

        changeStack.push_back(type);
        return changeStack.back();

    }

    void ClearChanges() {

        CUP_FUNCTION();

        unsaved = false;
        changeStack.clear();

    }

    void ResetUnsavedChanges() { unsaved = false; }
    bool UnsavedChanges() { return unsaved; }

}
