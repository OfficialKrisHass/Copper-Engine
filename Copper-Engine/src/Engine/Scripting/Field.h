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
			CopperObject


		};

		Type type;
		std::string name;

		void SetMonoField(MonoClassField* field) { if (!field) return; this->field = field; }

	private:
		MonoClassField* field;


	};

}