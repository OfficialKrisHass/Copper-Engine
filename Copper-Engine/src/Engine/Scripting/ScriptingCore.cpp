#include "cupch.h"
#include "ScriptingCore.h"

#include "Engine/Utilities/FileUtils.h"

#include "Engine/Scripting/MonoUtils.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Copper::Scripting {

	namespace InternalCalls {

		static void EditorLog(MonoString* msg);
		static void EditorLogWarn(MonoString* msg);
		static void EditorLogError(MonoString* msg);

	}

	struct ScriptingCoreData {

		MonoDomain* root;
		MonoDomain* app;

		MonoAssembly* apiAssembly;
		MonoImage* apiAssemblyImage;

		std::filesystem::path projectPath;
		MonoAssembly* projectAssembly;
		MonoImage* projectAssemblyImage;

	};

	ScriptingCoreData data;

	void SetupInternalCalls();

	void LogClasses();

	void Initialize() {

		//Initialize Mono
		mono_set_assemblies_path("lib/mono/lib");
		data.root = mono_jit_init("CUJITRuntime");

		//Load the ScriptingAPI
		data.app = mono_domain_create_appdomain("CUScriptRuntime", nullptr);
		mono_domain_set(data.app, true);

		data.apiAssembly = MonoUtils::LoadAssembly("assets/ScriptAPI/Copper-ScriptingAPI.dll");
		data.apiAssemblyImage = mono_assembly_get_image(data.apiAssembly);

		//Setup ScriptingAPI
		SetupInternalCalls();

	}
	void Shutdown() {

		data.app = nullptr;
		data.root = nullptr;

	}

	void LoadProjectAssembly(std::filesystem::path path) {

		data.projectPath = path;
		data.projectAssembly = MonoUtils::LoadAssembly(path);
		data.projectAssemblyImage = mono_assembly_get_image(data.projectAssembly);

	}
	void Reload(bool projectAlreadyLoaded) {

		mono_domain_set(mono_get_root_domain(), false);
		mono_domain_unload(data.app);

		//Load the Scripting API
		data.app = mono_domain_create_appdomain("CUScriptRuntime", nullptr);
		mono_domain_set(data.app, true);

		data.apiAssembly = MonoUtils::LoadAssembly("assets/ScriptAPI/Copper-ScriptingAPI.dll");
		data.apiAssemblyImage = mono_assembly_get_image(data.apiAssembly);

		//Load the Project Assembly
		if (!projectAlreadyLoaded) LoadProjectAssembly(data.projectPath);

		LogClasses();

	}

	void SetupInternalCalls() {

		mono_add_internal_call("Copper.InternalCalls::Log", InternalCalls::EditorLog);
		mono_add_internal_call("Copper.InternalCalls::LogWarn", InternalCalls::EditorLogWarn);
		mono_add_internal_call("Copper.InternalCalls::LogError", InternalCalls::EditorLogError);

	}

	void LogClasses() {

		MonoImage* image = mono_assembly_get_image(data.projectAssembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numOfTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int i = 0; i < numOfTypes; i++) {

			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			Log("{0}.{1}", nameSpace, name);

		}

	}

}