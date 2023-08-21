#include "cupch.h"
#include "Entity.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/Scene.h"

namespace Copper {

	Entity::Entity(InternalEntity* entity) {

		if (!entity) return;

		id = entity->id;
		scene = entity->scene;

	}

	bool Entity::operator==(const Entity& other) const { return id == other.id && scene == other.scene; }
	bool Entity::operator==(InternalEntity* other) const { return id == other->id && scene == other->scene; }

	InternalEntity* Entity::operator->() {

		return scene->GetEntityFromID(id);

	}

	Entity::operator InternalEntity* () const {

		return scene->GetEntityFromID(id);

	}
	Entity::operator bool() const { return id != INVALID_ENTITY_ID && scene != nullptr; }

}