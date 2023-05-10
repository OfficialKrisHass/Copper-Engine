#pragma once

#include "Engine/Core/Core.h"

#include <bitset>

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

		template<typename T> T* AddComponent();
		template<typename T> T* GetComponent();
		template<typename T> bool HasComponent();
		template<typename T> void RemoveComponent();

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

}