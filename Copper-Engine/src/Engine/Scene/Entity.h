#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Core/Engine.h"

#include <yaml-cpp/yaml.h>

namespace Copper {

	inline const int maxComponents = 32;
	inline const uint32_t invalidID = 4'294'967'295;

	class Transform;
	class Scene;

	class InternalEntity {

		friend class Registry;
		friend class Scene;
		friend class OldSceneDeserialization;
		friend class Entity;
		friend struct YAML::convert<InternalEntity*>;

	public:
		InternalEntity() = default;

		std::string name = "";

		template<typename T> T* AddComponent() {

			return scene->registry.AddComponent<T>(id);

		}
		template<typename T> T* GetComponent() {

			return scene->registry.GetComponent<T>(id);

		}
		template<typename T> bool HasComponent() {

			return scene->registry.HasComponent<T>(id);

		}
		template<typename T> void RemoveComponent() {

			scene->registry.RemoveComponent<T>(id);

		}

		Transform* GetTransform() const { return transform; }
		uint32_t ID() const { return id; }

		bool operator==(const InternalEntity& other) const { return id == other.id && scene == other.scene; }

		operator bool() const { return id != invalidID && scene != nullptr; }
		operator uint32_t() const { return id; };
		operator int32_t() const { return id; };

	private:
		uint32_t id = invalidID;
		Scene* scene = nullptr;
		Transform* transform = nullptr;

		std::bitset<maxComponents> cMask;

		void Invalidate() {

			id = invalidID;
			name = "";
			scene = nullptr;
			transform = nullptr;
			cMask.reset();

		}

	};

	class Entity {

		friend class Scene;

	public:
		Entity() = default;
		Entity(InternalEntity* entity);

		inline uint32_t ID() const { return id; }

		operator InternalEntity* () const;
		operator bool() const;

		InternalEntity* operator->();

	private:
		uint32_t id = invalidID;
		Scene* scene = nullptr;

	};

	YAML::Emitter& operator<<(YAML::Emitter& out, InternalEntity* entity);

}

namespace YAML {

	using namespace Copper;

	template<> struct convert<InternalEntity*> {

		static Node encode(const InternalEntity*& entity);
		static bool decode(const Node& node, InternalEntity*& entity);

	};

}