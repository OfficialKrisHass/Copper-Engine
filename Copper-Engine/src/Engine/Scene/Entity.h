#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	class Entity {

		friend class Scene;

	public:
		Entity() = default;
		Entity(class InternalEntity* entity);

		inline uint32_t ID() const { return m_id; }

		bool operator==(const Entity& other) const;
		bool operator==(InternalEntity* other) const;
		bool operator!=(const Entity& other) const { return !(*this == other); }
		bool operator!=(InternalEntity* other) const { return !(*this == other); }

		InternalEntity* operator->();

		operator InternalEntity*() const;
		operator bool() const;

	private:
		uint32_t m_id = 4'294'967'295;
		Scene* m_scene = nullptr;

	};

	inline std::ostream& operator<<(std::ostream& os, const Entity& entity) {

		return os << entity;

	}

}