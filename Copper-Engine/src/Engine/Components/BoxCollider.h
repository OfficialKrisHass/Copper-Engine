#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"
#include "Engine/Components/Collider.h"

#include "Engine/Scene/Scene.h"

#ifdef CU_EDITOR
namespace Editor { class Properties; }
#endif

namespace Copper {

    class BoxCollider : public Collider {

        friend Scene;

#ifdef CU_EDITOR
        friend Editor::Properties;
#endif

    public:
        const Vector3 GetSize() const { return m_size; }

        void SetSize(const Vector3& value) {

            CUP_FUNCTION();

            if (m_size == value) return;
            m_size = value;

            if (IsSceneRuntimeRunning())
                RecreateShape();

        }

    private:
        Vector3 m_size = Vector3::one;

        physx::PxShape* CreateShape() const override;

    };
    
}
