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

		operator bool() const { return valid; }

	private:
		Entity entity = nullptr;
		Transform* transform = nullptr;

	protected:
		bool valid = false;

	};

}