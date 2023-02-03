#include "cupch.h"
#include "Entity.h"

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
	Entity::operator bool() const { return scene != nullptr && *scene->GetEntityFromID(id); }

	InternalEntity* Entity::operator->() {

		return scene->GetEntityFromID(id);

	}

	YAML::Emitter& operator<<(YAML::Emitter& out, InternalEntity* entity) {

		out << entity->ID();
		return out;

	}

}

namespace YAML {

	using namespace Copper;

	Node convert<InternalEntity*>::encode(const InternalEntity*& entity) {

		Node node;

		node.push_back(entity->ID());

		return node;

	}
	bool convert<InternalEntity*>::decode(const Node& node, InternalEntity*& entity) {

		uint32_t objID = node.as<uint32_t>();

		if (objID == invalidID) { return true; } //we need to return true because if we don't, YAML crashes
		if (objID >= (uint32_t) GetNumOfEntities()) { entity = CreateEntityFromID(objID); return true; }

		entity = GetEntityFromID(node.as<uint32_t>());

		return true;

	}

}