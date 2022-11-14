#include "cupch.h"

#include <mono/jit/jit.h>

#include <mono/metadata/assembly.h>

namespace Copper::ScriptEngine {

	struct ScriptEngineData {

		MonoDomain* root;
		MonoDomain* app;

	};

	ScriptEngineData data;

	char* ReadAssembly(std::string path, uint32_t* outSize);

	void Initialize() {

		mono_set_assemblies_path("mono/lib");

		data.root = mono_jit_init("CopperScriptRuntime");
		if (!data.root) { LogError("Mono Failed to Initialize Root Domain"); return; }

		data.app = mono_domain_create_appdomain("CopperAppDomain", nullptr);
		mono_domain_set(data.app, true);

		//Load Assembly
		uint32_t fileSize = 0;
		char* fileData = ReadAssembly("assets/Scripts/Script-Core.dll", &fileSize);

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK) { LogError("Failed to Read Assembly.\n\n{0}", mono_image_strerror(status)); return; }

		MonoAssembly* assembly = mono_assembly_load_from_full(image, "assets/Scripts/Script-Core.dll", &status, 0);
		mono_image_close(image);

		delete[] fileData;

		//Print Assembly Types
		image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numOfTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int i = 0; i < numOfTypes; i++) {

			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			Log("{0}.{1}", nameSpace, name);

		}

		//Get an Instance of our Class
		MonoClass* klass = mono_class_from_name(image, "Copper", "Main");
		if (!klass) { LogError("Failed to Get a Class Reference!"); return; }

		MonoObject* instance = mono_object_new(data.app, klass);
		if (!instance) { LogError("Failed to Create a class Instance!"); return; }

		mono_runtime_object_init(instance);

		//Call a parameterless Function
		MonoClass* instanceClass = mono_object_get_class(instance);
		MonoMethod* method = mono_class_get_method_from_name(instanceClass, "PrintMessage", 0);
		if (!method) { LogError("Failed to get C# Method!"); return; }

		MonoObject* exception = nullptr;
		mono_runtime_invoke(method, instance, nullptr, &exception);

		//Call a Function with a float parameter
		method = mono_class_get_method_from_name(instanceClass, "PrintInt", 1);
		if (!method) { LogError("Failed to get C# Method!"); return; }

		exception = nullptr;

		float test = 10.789f;
		void* param = &test;

		mono_runtime_invoke(method, instance, &param, &exception);

		//Call a Function with a string parameter
		MonoString* monoString = mono_string_new(data.app, "This A String made in C++");
		method = mono_class_get_method_from_name(instanceClass, "PrintMessageCustom", 1);

		exception = nullptr;
		param = monoString;

		mono_runtime_invoke(method, instance, &param, &exception);

	}

	void Shutdown() {

		//

	}

	char* ReadAssembly(std::string path, uint32_t* outSize) {

        std::ifstream stream(path, std::ios::binary | std::ios::ate);

		if (!stream) { LogError("Failed to Read Assembly File.\nPath: {0}", path); return nullptr; }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        uint32_t size = end - stream.tellg();

		if (size == 0) { LogError("Assembly is empty.\nPath: {0}", path); return nullptr; }

        char* buffer = new char[size];
        stream.read((char*) buffer, size);
        stream.close();

        *outSize = size;
        return buffer;

	}

}