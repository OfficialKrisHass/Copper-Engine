#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

#include "Engine/Scene/Scene.h"

#define COLLIDER_TYPES 3

namespace physx { class PxShape; }

COMPONENT_FORWARD_DECL()

namespace Copper {

    class RigidBody;

    class Collider : public Component {

        COMPONENT_FRIEND_CLASSES();
        friend class Registry;

        friend RigidBody;

    public:
        enum Type : uint8 {

            None,

            Box,
            Sphere,
            Capsule,

        };

        void RecreateShape();

        // Getters

        inline Type GetType() const { return m_type; }
        inline RigidBody* GetRigidBody() const { return m_rb; }

        inline bool GetTrigger() const { return m_trigger; }
        inline const Vector3& GetCenter() const { return m_center; }

        // Setters

        void SetTrigger(bool value) {

            CUP_FUNCTION();

            m_trigger = value;
            RecreateShape();

        }
        void SetCenter(const Vector3& value) {

            CUP_FUNCTION();

            m_center = value;
            RecreateShape();

        }

    protected:
        Type m_type = Type::None;
        RigidBody* m_rb = nullptr;

        bool m_trigger = false;
        Vector3 m_center = Vector3::zero;

    private:
        virtual physx::PxShape* CreateShape() const = 0;

    };

}
