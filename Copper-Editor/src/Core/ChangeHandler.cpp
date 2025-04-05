#include "ChangeHandler.h"

#include "Core/EditorApp.h"

namespace Editor {

    std::vector<Change> changes;
    int32 changeIndex = -1;

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

    void RedoChange() {

        CUP_FUNCTION();

        CU_ASSERT(changeIndex >= -1, "Change index went too low: {}", changeIndex);

        if (changeIndex + 1 >= changes.size()) return;
        changeIndex++;

        Change& change = changes[changeIndex];
        change.ResetIndex();

        switch (change.type) {

        case Change::Type::EntityMoved: {

            uint32 id;
            Vector3 value;
            change >> id >> value >> value;

            CU_ASSERT(GetEntityFromID(id) != nullptr, "Invalid entity in Change data. ID: {}, change index: {}", id, changeIndex);
            GetEntityFromID(id)->GetTransform()->SetPosition(value);

            break;

        }
        case Change::Type::EntityRotated: {

            uint32 id;
            Quaternion value;
            change >> id >> value >> value;

            CU_ASSERT(GetEntityFromID(id) != nullptr, "Invalid entity in Change data. ID: {}, change index: {}", id, changeIndex);
            GetEntityFromID(id)->GetTransform()->SetRotation(value);

            break;

        }
        case Change::Type::EntityScaled: {

            uint32 id;
            Vector3 value;
            change >> id >> value >> value;

            CU_ASSERT(GetEntityFromID(id) != nullptr, "Invalid entity in Change data. ID: {}, change index: {}", id, changeIndex);
            GetEntityFromID(id)->GetTransform()->SetScale(value);

            break;

        }                                       
        default: break;

        }


    }
    void UndoChange() {

        CUP_FUNCTION();

        if (changeIndex < 0 || changeIndex >= changes.size()) return; // Since it's a uint32, the value will wrap around to the max value, therefore bigger than the size

        Change& change = changes[changeIndex];
        change.ResetIndex();

        switch (change.type) {
        
        case Change::Type::EntityMoved: {

            uint32 id;
            Vector3 value;
            change >> id >> value;

            CU_ASSERT(GetEntityFromID(id) != nullptr, "Invalid entity in Change data. ID: {}, change index: {}", id, changeIndex);
            GetEntityFromID(id)->GetTransform()->SetPosition(value);

            break;

        }
        case Change::Type::EntityRotated: {

            uint32 id;
            Quaternion value;
            change >> id >> value;

            CU_ASSERT(GetEntityFromID(id) != nullptr, "Invalid entity in Change data. ID: {}, change index: {}", id, changeIndex);
            GetEntityFromID(id)->GetTransform()->SetRotation(value);

            break;

        }
        case Change::Type::EntityScaled: {

            uint32 id;
            Vector3 value;
            change >> id >> value;

            CU_ASSERT(GetEntityFromID(id) != nullptr, "Invalid entity in Change data. ID: {}, change index: {}", id, changeIndex);
            GetEntityFromID(id)->GetTransform()->SetScale(value);

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
