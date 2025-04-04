#include "ChangeHandler.h"

#include "Core/EditorApp.h"

namespace Editor {

    std::vector<Change> changes;
    uint32 changeIndex = 0;

    bool unsaved = false;

    Change& AddChange(Change::Type type) {

        CUP_FUNCTION();

        if (!unsaved) {

            unsaved = true;

            std::string title = GetWindowTitle(); 
            title += '*';
            Input::SetWindowTitle(title);

        }

        changeIndex = changes.size();
        changes.push_back(type);

        return changes.back();

    }

    void UndoChange() {

        CUP_FUNCTION();

        if (changes.size() == 0) return;
        if (changeIndex >= changes.size()) return; // Since it's a uint32, the value will wrap around to the max value, therefore bigger than the size

        Change& change = changes[changeIndex];

        switch (change.type) {
        
        case Change::Type::EntityTransformed: {

            uint32 id;
            Vector3 pos, scale;
            Quaternion rot;
            change >> id >> pos >> rot >> scale;

            InternalEntity* entity = GetEntityFromID(id);

            entity->GetTransform()->SetPosition(pos);
            entity->GetTransform()->SetRotation(rot);
            entity->GetTransform()->SetScale(scale);

            break;

        }
        default: break;

        }

        changeIndex--;

    }
    void ClearChanges() {

        CUP_FUNCTION();

        unsaved = false;
        changes.clear();

    }

    void ResetUnsavedChanges() { unsaved = false; }
    bool UnsavedChanges() { return unsaved; }

}
