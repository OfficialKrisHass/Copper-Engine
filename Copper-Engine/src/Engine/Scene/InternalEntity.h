#pragma once

#include "Engine/Core/Core.h"

#include <bitset>

#define INVALID_ENTITY_ID 4'294'967'295
#define MAX_ENTITY_COMPONENTS 32

#define ENTITY_DEFAULT_PROPERTIES_DECLARATION const Vector3& position = Vector3::zero, const Quaternion& rotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f), const Vector3& scale = Vector3::one, const std::string& name = "Entity"
#define ENTITY_PROPERTIES_DECLARATION const Vector3& position, const Quaternion& rotation, const Vector3& scale, const std::string& name

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

		Transform* GetTransform() const { return m_transform; }
		uint32_t ID() const { return m_id; }
		const uint32_t* IDPointer() const { return &m_id; }

		bool operator==(const InternalEntity& other) const { return m_id == other.m_id && m_scene == other.m_scene; }

		explicit operator bool() const { return m_id != INVALID_ENTITY_ID && m_scene != nullptr; }
		explicit operator uint32_t() const { return m_id; };
		explicit operator int32_t() const { return m_id; };

	private:
		uint32_t m_id = INVALID_ENTITY_ID;
		Scene* m_scene = nullptr;
		Transform* m_transform = nullptr;

		std::bitset<MAX_ENTITY_COMPONENTS> m_cMask;

		void Invalidate() {

			m_id = INVALID_ENTITY_ID;
			name = "";
			m_scene = nullptr;
			m_transform = nullptr;
			m_cMask.reset();

		}

	};

	inline std::ostream& operator<<(std::ostream& os, const InternalEntity& entity) {

		return os << entity.name << " (" << entity.m_id << ")";

	}

	InternalEntity* CreateEntity(ENTITY_DEFAULT_PROPERTIES_DECLARATION);
	InternalEntity* CreateEntityFromID(uint32 id, ENTITY_DEFAULT_PROPERTIES_DECLARATION, bool returnIfExists = true);
	InternalEntity* GetEntityFromID(uint32 id);
	void RemoveEntity(InternalEntity* entity);
	void RemoveEntityFromID(uint32 id);

	uint32 GetNumOfEntities();

}