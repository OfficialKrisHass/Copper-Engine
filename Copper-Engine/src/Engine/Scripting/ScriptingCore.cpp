#include "cupch.h"
#include "ScriptingCore.h"

#include "Engine/Utilities/FileUtils.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/InternalCalls.cpp"

#include <CopperECS/Registry.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Copper::Scripting {

	struct ScriptingCoreData {

		MonoDomain* root;
		MonoDomain* app;

		MonoAssembly* apiAssembly;
		MonoImage* apiAssemblyImage;

		std::filesystem::path projectPath;
		MonoAssembly* projectAssembly;
		MonoImage* projectAssemblyImage;

		MonoClass* componentClass;

		std::vector<std::string> scriptComponents;

	};

	ScriptingCoreData data;

	void SetupInternalCalls();

	void InitScriptComponents();

	void Initialize() {

		//Initialize Mono
		mono_set_assemblies_path("lib/mono/lib");
		data.root = mono_jit_init("CUJITRuntime");

		//Load the ScriptingAPI
		data.app = mono_domain_create_appdomain("CUScriptRuntime", nullptr);
		mono_domain_set(data.app, true);

		data.apiAssembly = MonoUtils::LoadAssembly("assets/ScriptAPI/Copper-ScriptingAPI.dll");
		data.apiAssemblyImage = mono_assembly_get_image(data.apiAssembly);

		data.componentClass = mono_class_from_name(data.apiAssemblyImage, "Copper", "Component");

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
		data.componentClass = mono_class_from_name(data.apiAssemblyImage, "Copper", "Component");

		//Load the Project Assembly
		if (!projectAlreadyLoaded) LoadProjectAssembly(data.projectPath);

		InitScriptComponents();

	}

	void SetupInternalCalls() {
		
		//Logging
		mono_add_internal_call("Copper.InternalCalls::Log", InternalCalls::EditorLog);
		mono_add_internal_call("Copper.InternalCalls::LogWarn", InternalCalls::EditorLogWarn);
		mono_add_internal_call("Copper.InternalCalls::LogError", InternalCalls::EditorLogError);

		//Object
		mono_add_internal_call("Copper.InternalCalls::GetObjectName", InternalCalls::GetObjectName);
		mono_add_internal_call("Copper.InternalCalls::SetObjectName", InternalCalls::SetObjectName);

		//Transform
		mono_add_internal_call("Copper.InternalCalls::GetPosition", InternalCalls::GetPosition);
		mono_add_internal_call("Copper.InternalCalls::GetRotation", InternalCalls::GetRotation);
		mono_add_internal_call("Copper.InternalCalls::GetScale", InternalCalls::GetScale);
		mono_add_internal_call("Copper.InternalCalls::SetPosition", InternalCalls::SetPosition);
		mono_add_internal_call("Copper.InternalCalls::SetRotation", InternalCalls::SetRotation);
		mono_add_internal_call("Copper.InternalCalls::SetScale", InternalCalls::SetScale);


	}

	void InitScriptComponents() {

		data.scriptComponents.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(data.projectAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numOfTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int i = 0; i < numOfTypes; i++) {

			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(data.projectAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(data.projectAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			if (std::string(name) == "<Module>") continue;

			MonoClass* scriptClass = mono_class_from_name(data.projectAssemblyImage, nameSpace, name);
			if (mono_class_is_subclass_of(scriptClass, data.componentClass, false)) {

				std::string fullName;
				if (strlen(nameSpace) != 0) fullName = fmt::format("{}.{}", nameSpace, name);
				else fullName = name;

				data.scriptComponents.push_back(fullName);

				//Log("{0}.{1}", nameSpace, name);

			}

		}

	}

	MonoObject* AddScriptComponent(int32_t obj, const std::string& name) {

		std::string scriptName = name;
		std::string nameSpace = MonoUtils::RemoveNamespace(scriptName);

		MonoClass* script = mono_class_from_name(data.projectAssemblyImage, nameSpace.c_str(), scriptName.c_str());
		if (!script) { LogError("Failed to Get {} Class Reference", name); return nullptr; }

		MonoObject* instance = mono_object_new(data.app, script);
		if (!instance) { LogError("Failed to Instantiate {}", name); return nullptr; }

		mono_runtime_object_init(instance);

		MonoMethod* componentConstructor = mono_class_get_method_from_name(data.componentClass, ".ctor", 1);
		if (!componentConstructor) { LogError("Failed to get the Component Constructor!"); return nullptr; }

		void* param = &obj;
		MonoObject* exception = nullptr;
		mono_runtime_invoke(componentConstructor, instance, &param, &exception);

		return instance;

	}

	std::vector<std::string> GetScriptComponents() { return data.scriptComponents; }

	MonoDomain* GetRootDomain() { return data.root; }
	MonoDomain* GetAppDomain() { return data.app; }

}