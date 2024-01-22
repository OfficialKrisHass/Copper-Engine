#include "cupch.h"
#include "Entity.h"

#include "Engine/Scene/Scene.h"

namespace Copper {

	Entity::Entity(InternalEntity* entity) {

		if (!entity) return;

		m_id = entity->m_id;
		m_scene = entity->m_scene;

	}

	bool Entity::operator==(const Entity& other) const { return m_id == other.m_id && m_scene == other.m_scene; }
	bool Entity::operator==(InternalEntity* other) const { return m_id == other->m_id && m_scene == other->m_scene; }

	InternalEntity* Entity::operator->() { return m_scene->GetEntityFromID(m_id); }

	Entity::operator InternalEntity* () const { return m_scene->GetEntityFromID(m_id); }
	Entity::operator bool() const { return m_id != INVALID_ENTITY_ID && m_scene != nullptr; }

}