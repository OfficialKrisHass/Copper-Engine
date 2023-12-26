#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	class InternalEntity;
	class Scene;

	std::ostream& operator<<(std::ostream& os, const class InternalEntity& entity);

	class Entity {

		friend Scene;

	public:
		Entity() = default;
		Entity(InternalEntity* entity);

		inline uint32_t ID() const { return id; }

		bool operator==(const Entity& other) const;
		bool operator==(InternalEntity* other) const;
		bool operator!=(const Entity& other) const { return !(*this == other); }
		bool operator!=(InternalEntity* other) const { return !(*this == other); }

		InternalEntity* operator->();

		operator InternalEntity*() const;
		operator bool() const;

	private:
		uint32_t id = 4'294'967'295;
		Scene* scene = nullptr;

	};

	inline std::ostream& operator<<(std::ostream& os, const Entity& entity) {

		return os << entity;

	}

}