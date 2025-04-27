#pragma once

#include "Engine/Events/Event.h"

#include "Engine/Input/KeyCodes.h"

namespace Copper {

    class MouseEvent : public Event {

    public:
        MouseEvent() = default;
        MouseEvent(Vector2I mouseCoords) : mouseCoords(mouseCoords) {}

        Vector2I mouseCoords = Vector2I::zero;
        KeyCode button = KeyCode::None;

    };

}
