#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/InternalEntity.h"
#include "Engine/Scene/Entity.h"

#ifdef CU_EDITOR
namespace Editor { struct Project; class SceneCamera; }
#endif

namespace Copper {

	class Component {

		friend class Registry;
		friend class Scene;
		friend class OldSceneVersionSerializer;

	#ifdef CU_EDITOR
		friend Editor::Project;
		friend Editor::SceneCamera;
	#endif

	public:
		Entity GetEntity() const { return m_entity; }
		class Transform* GetTransform() const { return m_transform; }

		operator bool() const { return m_valid; }

	private:
		Entity m_entity = nullptr;
		Transform* m_transform = nullptr;

		bool m_valid = false;

	};

}

#include "Engine/Components/Transform.h"