#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

#ifdef CU_EDITOR
namespace YAML { class Emitter; }
namespace Editor {

    class Project;
    namespace SceneSerializer {

        void SerializeEntityComponents(Copper::InternalEntity* entity, YAML::Emitter& out);

    }

}
#endif

namespace Copper {

    class Camera : public Component {

        friend class Scene;
#ifdef CU_EDITOR
        friend Editor::Project;
        friend void Editor::SceneSerializer::SerializeEntityComponents(Copper::InternalEntity *entity, YAML::Emitter &out);
#endif

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
