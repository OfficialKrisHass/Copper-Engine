#pragma once

extern "C" {

	typedef struct _MonoClassField MonoClassField;

}

namespace Copper {

	class ScriptComponent;

	struct ScriptField {

		friend ScriptComponent;

		enum class Type : uint32_t {

			None,
			Int,
			UInt,
			Float,

			Vector2,
			Vector3,

			Entity,
			Transform,

		};

		std::string name;
		Type type = Type::None;

		void SetMonoField(MonoClassField* field) { if (!field) return; this->field = field; }

	private:
		MonoClassField* field = nullptr;

	};

	inline uint32_t FieldSize(const ScriptField::Type type) {

		typedef ScriptField::Type Type;

		switch (type) {

		case Type::None: break;

		case Type::Int:
		case Type::UInt:
		case Type::Float: return 4;

		case Type::Vector2: return 2 * 4;
		case Type::Vector3: return 3 * 4;

		case Type::Entity:
		case Type::Transform: return 8;

		}

		LogError("Couldn't get Type ({}) size!", (uint32_t) type);
		return 0;

	}

}