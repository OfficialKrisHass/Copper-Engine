#pragma once

#include <Copper.h>

#include <Engine/Components/Camera.h>

namespace Editor {

    class Viewport;

    class SceneCamera : public Copper::Camera {

        friend class Project;

    public:
        SceneCamera() = default;
        SceneCamera(Copper::UVector2I size, Viewport* viewport);

        void Update();

        void UpdateRotation();
        
        inline void SetCanLook(bool can) { this->m_canLook = can; }
        
        float speed = 0.1f;
        float sensitivity = 100.0f;

    private:
        Viewport* m_viewport = nullptr;

        float m_multiplier = 1.0f;

        bool m_firstClick = true;
        bool m_canLook = false;

        // Locked cursor position. ImGui uses the virtual cursor which causes it to "move" which will make it move outside the
        // viewport, highlight UI elements and lock the cursor in the locked state.
        Copper::Vector2I m_cursorPosition;

        Copper::Quaternion m_rotation;

    };

}
