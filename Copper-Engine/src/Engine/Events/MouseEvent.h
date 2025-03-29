#pragma once

#include "Engine/Events/Event.h"

#include "Engine/Input/KeyCodes.h"

namespace Copper {

    class MouseEvent : public Event {

    public:
        MouseEvent() = default;
        MouseEvent(UVector2I mouseCoords) : mouseCoords(mouseCoords) {}

        UVector2I mouseCoords = UVector2I::zero;
        KeyCode button = KeyCode::None;

    };

}
