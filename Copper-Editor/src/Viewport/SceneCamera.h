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

        bool m_firstClick = true;
        bool m_canLook = false;

        float m_rotX = 0.0f;
        float m_rotY = 0.0f;

    };

}
