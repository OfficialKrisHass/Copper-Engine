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
		Type type;

		void SetMonoField(MonoClassField* field) { if (!field) return; this->field = field; }

	private:
		MonoClassField* field;


	};

}