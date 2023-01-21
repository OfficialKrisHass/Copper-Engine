#include "cupch.h"
#include "MonoUtils.h"

#include "Engine/Utilities/FileUtils.h"

#include "Engine/Scripting/ScriptingCore.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>

namespace Copper::Scripting::MonoUtils {

	MonoAssembly* LoadAssembly(std::filesystem::path path) {

		uint32_t dataSize;
		char* data = Utilities::ReadFileBinary(path.string(), &dataSize);

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(data, dataSize, 1, &status, 0);
		if (status != MONO_IMAGE_OK) { LogError("Failed to Read Assembly {0}.\n\n{1}", path, mono_image_strerror(status)); return nullptr; }

		MonoAssembly* assembly = mono_assembly_load_from_full(image, path.string().c_str(), &status, 0);
		mono_image_close(image);

		delete[] data;
		return assembly;


	}

	std::string MonoToString(MonoString* string) {

		if (string == nullptr || mono_string_length(string) == 0) { LogError("MonoString Can't be invalid"); return ""; }

		MonoError error;
		char* utf8 = mono_string_to_utf8_checked(string, &error);

		std::string ret(utf8);
		mono_free(utf8);

		return ret;

	}
	MonoString* StringToMono(const std::string& string) {

		if (string.size() == 0) { LogError("String can't be Invalid"); return nullptr; }

		MonoString* ret = mono_string_new(Scripting::GetAppDomain(), string.c_str());

		return ret;

	}

	std::string RemoveNamespace(std::string& name) {

		size_t pos = name.find_last_of('.');
		if (pos == std::string::npos) return ""; //Name doesn't have a Namespace

		std::string nameSpace = name.substr(0, pos);
		name.erase(0, pos + 1);

		return nameSpace;

	}

	FieldAccessibility GetFieldAccessibility(MonoClassField* field) {

		return (mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK) == MONO_FIELD_ATTR_PUBLIC ? FieldAccessibility::Public : FieldAccessibility::Private;

	}
	ScriptField::Type TypeFromString(const std::string& string) {

		if      (string == "System.Int32")   return ScriptField::Type::Int;
		else if (string == "System.UInt32")  return ScriptField::Type::UInt;
		else if (string == "System.Single")  return ScriptField::Type::Float;
		else if (string == "Copper.CopperObject")  return ScriptField::Type::CopperObject;

		return ScriptField::Type::None;

	}

}