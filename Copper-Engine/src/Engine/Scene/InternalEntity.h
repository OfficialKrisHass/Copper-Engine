#pragma once

#include "Engine/Core/Core.h"

#include <bitset>

#define INVALID_ENTITY_ID 4'294'967'295
#define MAX_ENTITY_COMPONENTS 32

namespace Copper {

	class Transform;
	class Scene;

	std::ostream& operator<<(std::ostream& os, const class InternalEntity& entity);

	class InternalEntity {

		friend class Registry;
		friend class Scene;
		friend class OldSceneDeserialization;
		friend class Entity;

		friend std::ostream& operator<<(std::ostream& os, const InternalEntity& entity);

	public:
		InternalEntity() = default;

		std::string name = "";

		template<typename T> T* AddComponent();
		template<typename T> T* GetComponent();
		template<typename T> bool HasComponent();
		template<typename T> void RemoveComponent();

		void* GetComponent(int componentID);
		bool HasComponent(int componentID);
		void RemoveComponent(int componentID);

		Transform* GetTransform() const { return transform; }
		uint32_t ID() const { return id; }
		const uint32_t* IDPointer() const { return &id; }

		bool operator==(const InternalEntity& other) const { return id == other.id && scene == other.scene; }

		explicit operator bool() const { return id != INVALID_ENTITY_ID && scene != nullptr; }
		explicit operator uint32_t() const { return id; };
		explicit operator int32_t() const { return id; };

	private:
		uint32_t id = INVALID_ENTITY_ID;
		Scene* scene = nullptr;
		Transform* transform = nullptr;

		std::bitset<MAX_ENTITY_COMPONENTS> cMask;

		void Invalidate() {

			id = INVALID_ENTITY_ID;
			name = "";
			scene = nullptr;
			transform = nullptr;
			cMask.reset();

		}

	};

	inline std::ostream& operator<<(std::ostream& os, const InternalEntity& entity) {

		return os << entity.name << " (" << entity.id << ")";

	}

}