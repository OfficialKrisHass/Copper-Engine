#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

#include "Engine/Scene/Scene.h"

#define COLLIDER_TYPES 3

namespace physx { class PxShape; }

#ifdef CU_EDITOR
namespace Editor { class Properties; }
#endif

namespace Copper {

    class RigidBody;

	class Collider : public Component {

		friend class Registry;
		friend Scene;

		friend RigidBody;

#ifdef CU_EDITOR
        friend Editor::Properties;
#endif

	public:
		enum Type : uint8 {

			None,

			Box,
			Sphere,
			Capsule,

		};

        // Getters

        inline Type GetType() const { return m_type; }
        inline RigidBody* GetRigidBody() const { return m_rb; }

        inline bool GetTrigger() const { return m_trigger; }
        inline const Vector3& GetCenter() const { return m_center; }

        // Setters
        
        void SetTrigger(bool value) {

            CUP_FUNCTION();

            if (m_trigger == value) return;
            m_trigger = value;

            if (IsSceneRuntimeRunning())
                RecreateShape();

        }
        void SetCenter(const Vector3& value) {

            CUP_FUNCTION();

            if (m_center == value) return;
            m_center = value;

            if (IsSceneRuntimeRunning())
                RecreateShape();

        }

	protected:
		Type m_type = Type::None;
		RigidBody* m_rb = nullptr;

		bool m_trigger = false;
		Vector3 m_center = Vector3::zero;

        void RecreateShape();

	private:
		virtual physx::PxShape* CreateShape() const = 0;

	};

}
