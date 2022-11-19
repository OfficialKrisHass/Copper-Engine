#pragma once

#include "Engine/Core/Core.h"

extern "C" {

	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClass MonoClass;

}

namespace Copper {

	enum class Type {

		Invalid, Field, Property

	};
	enum class VariableType {

		None,
		Int,
		UInt,
		Float,
		Double,
		String,
		Char,
		Vector2,
		Vector3

	};

	class Variable {

	public:
		Variable() : name(""), varType(VariableType::None), type(Type::Invalid), script(nullptr) {}

		std::string name;
		VariableType varType;
		Type type;

		MonoClass* script;

		void GetValue(MonoObject* instance, void* out);
		void SetValue(MonoObject* instance, void* value);

	private:
		//

	};

}