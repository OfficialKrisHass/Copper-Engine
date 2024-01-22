#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scripting/Field.h"

#include <cupch.h>

extern "C" {

	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoString MonoString;
	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoClassField MonoClassField;

}

namespace Copper::Scripting::MonoUtils {

	//Enums
	enum class FieldAccessibility : uint8 {

		None = 0,
		Public = (1 << 0),
		Private = (1 << 1),

	};

	//Assembly
	MonoAssembly* LoadAssembly(const fs::path& path);

	//Strings
	std::string MonoToString(MonoString* string);
	MonoString* StringToMono(const std::string& string);

	std::string RemoveNamespace(std::string& name);

	//Fields
	FieldAccessibility GetFieldAccessibility(MonoClassField* field);
	ScriptField::Type TypeFromString(const std::string& string);

	bool IsBuiltinComponentField(const std::string& name);

	//Scripts
	void PrintExceptionDetails(MonoObject* exc);

}