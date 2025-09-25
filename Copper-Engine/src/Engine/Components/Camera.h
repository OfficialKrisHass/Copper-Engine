#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

COMPONENT_FORWARD_DECL()

namespace Copper {

    class Camera : public Component {

        COMPONENT_FRIEND_CLASSES();

    public:
        Camera() = default;
#ifdef CU_EDITOR
        Camera(const UVector2I& size) : size(size) {}
#endif

        float fov = 90.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;

        Matrix4 CreateViewMatrix() const;
        Matrix4 CreateProjectionMatrix() const;

#ifdef CU_EDITOR
        inline void Resize(const UVector2I& size) { this->size = size; }
#endif

    protected:
#ifdef CU_EDITOR
        UVector2I size = UVector2I(1280, 720);
#endif

    };

}
