#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Entity.h"

#ifdef CU_EDITOR
namespace Editor { struct Project; }
#endif

namespace Copper {

	class Component {

		friend class Registry;
		friend class Scene;
		friend class OldSceneVersionSerializer;

	#ifdef CU_EDITOR
		friend Editor::Project;
	#endif

	public:
		virtual void Added() {}
		virtual void Removed() {}

		InternalEntity* GetEntity() const { return entity; }
		Transform* GetTransform() const { return transform; }

	private:
		Entity entity = nullptr;
		Transform* transform = nullptr;

		bool valid = false;

	};

}