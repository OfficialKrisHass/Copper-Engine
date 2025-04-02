#pragma once

#include "Core/Change.h"

#include <Copper.h>

namespace Editor {

    using namespace Copper;

    Change& AddChange(Change::Type type);

    void UndoChange();
    void RedoChange();
    void ClearChanges();

    void ResetUnsavedChanges();
    bool UnsavedChanges();

}
