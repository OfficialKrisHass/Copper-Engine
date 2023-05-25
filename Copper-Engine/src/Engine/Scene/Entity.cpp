#include "cupch.h"
#include "Entity.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/Scene.h"

namespace Copper {

	Entity::Entity(InternalEntity* entity) {

		if (!entity)return;

		id = entity->id;
		scene = entity->scene;

	}

	Entity::operator InternalEntity* () const {

		return scene->GetEntityFromID(id);

	}
	Entity::operator bool() const { return id != invalidID && scene != nullptr; }

	InternalEntity* Entity::operator->() {

		return scene->GetEntityFromID(id);

	}

}