#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	class InternalEntity;
	class Scene;

	class Entity {

		friend Scene;

	public:
		Entity() = default;
		Entity(InternalEntity* entity);

		inline uint32_t ID() const { return id; }

		operator InternalEntity* () const;
		operator bool() const;

		InternalEntity* operator->();

	private:
		uint32_t id = 4'294'967'295;
		Scene* scene = nullptr;

	};

}