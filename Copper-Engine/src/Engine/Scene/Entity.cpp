#include "cupch.h"
#include "Entity.h"

#include "Engine/Scene/Scene.h"

namespace Copper {

    Entity::Entity(InternalEntity* entity) {

        CUP_FUNCTION();

        if (entity == nullptr) return;

        m_id = entity->m_id;

    }

    bool Entity::IsValid() const {

        CUP_FUNCTION();

        return GetScene()->GetEntityFromID(m_id) != nullptr;

    }

    bool Entity::operator==(const Entity& other) const { return m_id == other.m_id; }
    bool Entity::operator==(InternalEntity* other) const { return m_id == other->m_id; }

    InternalEntity* Entity::operator->() { return GetScene()->GetEntityFromID(m_id); }

    Entity::operator InternalEntity* () const { return GetScene()->GetEntityFromID(m_id); }

}
