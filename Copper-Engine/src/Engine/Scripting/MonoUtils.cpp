#include "cupch.h"
#include "MonoUtils.h"

#include "Engine/Utilities/FileUtils.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

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
	MonoString* StringToMono(std::string string, MonoDomain* domain) {

		if (string.size() == 0) { LogError("String can't be Invalid"); return nullptr; }

		MonoString* ret = mono_string_new(domain, string.c_str());

		return ret;

	}

}