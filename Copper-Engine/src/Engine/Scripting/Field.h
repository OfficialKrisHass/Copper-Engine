#pragma once

extern "C" {

	typedef struct _MonoClassField MonoClassField;

}

namespace Copper {

	class ScriptComponent;

	struct ScriptField {

		friend class ScriptComponent;

		enum class Type {

			None,
			Int,
			UInt,
			Float,

			Vector2,
			Vector3,

			Entity,
			Component

		};

		Type type;
		std::string name;

		void SetMonoField(MonoClassField* field) { if (!field) return; this->field = field; }

	private:
		MonoClassField* field;


	};
	struct ComponentScriptField : public ScriptField {

		friend class ScriptComponent;

		std::string componentName;
		bool isBuiltinComponent;

	};

}