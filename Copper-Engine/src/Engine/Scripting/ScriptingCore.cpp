#include "cupch.h"
#include "ScriptingCore.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Utilities/FileUtils.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

char domainName[] = "CUScriptRuntime";

namespace Copper::Scripting {

	namespace InternalCalls {
		
		void SetupInternalCalls();
		void Initialize();
	
	}

	struct ScriptingCoreData {

		MonoDomain* root;
		MonoDomain* app;

		MonoAssembly* apiAssembly;
		MonoImage* apiAssemblyImage;

		fs::path projectPath;
		MonoAssembly* projectAssembly;
		MonoImage* projectAssemblyImage;

		MonoClass* entityClass;
		MonoClass* componentClass;
		MonoClass* vector2Class;
		MonoClass* vector3Class;

		std::vector<std::string> scriptComponents;
		std::unordered_map<std::string, std::vector<ScriptField>> scriptFields;

		std::vector<MonoObject*> entities;

	};

	ScriptingCoreData data;

	void InitScriptComponents();
	void InitScriptFields(const std::string& fullName, MonoClass* scriptClass);

	void Initialize() {

		VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize the Scripting Engine");

		//Initialize Mono
		mono_set_assemblies_path("lib/mono/lib");
		data.root = mono_jit_init("CUJITRuntime");

		//Load the ScriptingAPI
		data.app = mono_domain_create_appdomain(domainName, nullptr);
		mono_domain_set(data.app, true);

		data.apiAssembly = MonoUtils::LoadAssembly("assets/ScriptAPI/Copper-ScriptingAPI.dll");
		data.apiAssemblyImage = mono_assembly_get_image(data.apiAssembly);

		data.componentClass = mono_class_from_name(data.apiAssemblyImage, "Copper", "Component");
		data.vector2Class = mono_class_from_name(data.apiAssemblyImage, "Copper", "Vector2");
		data.vector3Class = mono_class_from_name(data.apiAssemblyImage, "Copper", "Vector3");
		data.entityClass = mono_class_from_name(data.apiAssemblyImage, "Copper", "Entity");

		//Setup ScriptingAPI
		InternalCalls::SetupInternalCalls();

	}
	void Shutdown() {

		data.app = nullptr;
		data.root = nullptr;

	}

	bool LoadProjectAssembly(const fs::path& path) {

		data.projectPath = path;

		data.projectAssembly = MonoUtils::LoadAssembly(path);
		if (!data.projectAssembly)
			return false;

		data.projectAssemblyImage = mono_assembly_get_image(data.projectAssembly);

		return true;


	}
	bool Reload(const fs::path& path, bool initScriptComponents) {

		if (!path.empty()) data.projectPath = path;

		mono_domain_set(mono_get_root_domain(), false);
		mono_domain_unload(data.app);

		//Load the Scripting API
		data.app = mono_domain_create_appdomain(domainName, nullptr);
		mono_domain_set(data.app, true);

		data.apiAssembly = MonoUtils::LoadAssembly("assets/ScriptAPI/Copper-ScriptingAPI.dll");
		data.apiAssemblyImage = mono_assembly_get_image(data.apiAssembly);

		data.componentClass = mono_class_from_name(data.apiAssemblyImage, "Copper", "Component");
		data.vector2Class = mono_class_from_name(data.apiAssemblyImage, "Copper", "Vector2");
		data.vector3Class = mono_class_from_name(data.apiAssemblyImage, "Copper", "Vector3");
		data.entityClass = mono_class_from_name(data.apiAssemblyImage, "Copper", "Entity");

		//Load the Project Assembly
		if (!LoadProjectAssembly(data.projectPath))
			return false;

		InitScriptComponents();
		InternalCalls::Initialize();

		if (!initScriptComponents) return true;
		
		for (ScriptComponent* script : ComponentView<ScriptComponent>(GetScene())) {

			script->Init(script->name);

		}
		return true;

	}

	void InitScriptComponents() {

		data.scriptComponents.clear();
		data.scriptFields.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(data.projectAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numOfTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int i = 0; i < numOfTypes; i++) {
			
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(data.projectAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(data.projectAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			if (std::string(name) == "<Module>") continue;

			MonoClass* scriptClass = mono_class_from_name(data.projectAssemblyImage, nameSpace, name);
			if (!mono_class_is_subclass_of(scriptClass, data.componentClass, false)) continue;

			std::string fullName;
			if (strlen(nameSpace) != 0) fullName = fmt::format("{}.{}", nameSpace, name);
			else fullName = name;

			data.scriptComponents.push_back(fullName);
			InitScriptFields(fullName, scriptClass);

		}

	}
	void InitScriptFields(const std::string& fullName, MonoClass* scriptClass) {

		void* iter = nullptr;
		MonoClassField* field;
		while (field = mono_class_get_fields(scriptClass, &iter)) {

			if (MonoUtils::GetFieldAccessibility(field) != MonoUtils::FieldAccessibility::Public) continue;

			MonoType* type = mono_field_get_type(field);

			data.scriptFields[fullName].push_back(ScriptField());
			ScriptField& scriptField = data.scriptFields[fullName].back();
			scriptField.SetMonoField(field);

			scriptField.name = mono_field_get_name(field);
			scriptField.type = MonoUtils::TypeFromString(mono_type_get_name(type));

		}

	}

	MonoObject* AddScriptComponent(uint32_t eID, const std::string& name) {

		std::string scriptName = name;
		std::string nameSpace = MonoUtils::RemoveNamespace(scriptName);

		MonoClass* script = mono_class_from_name(data.projectAssemblyImage, nameSpace.c_str(), scriptName.c_str());
		if (!script) { LogError("Failed to Get {} C# Class", name); return nullptr; }

		MonoObject* instance = mono_object_new(data.app, script);
		if (!instance) { LogError("Failed to Instantiate {} C# class", name); return nullptr; }

		MonoMethod* componentConstructor = mono_class_get_method_from_name(data.componentClass, ".ctor", 1);
		if (!componentConstructor) { LogError("Failed to get the Component Constructor!"); return nullptr; }

		void* param = &eID;
		MonoObject* exception = nullptr;
		mono_runtime_invoke(componentConstructor, instance, &param, &exception);

		return instance;

	}

	MonoObject* GetScriptEntity(uint32_t eID) {

		if (data.entities.size() < eID + 1) {

			data.entities.resize(eID + 1, nullptr);

		}
		if (!data.entities[eID]) {

			MonoObject* entity = mono_object_new(data.app, data.entityClass);
			if (!entity) { LogError("Failed to Create C# Entity, ID: {}", eID); return nullptr; }

			MonoMethod* constructor = mono_class_get_method_from_name(data.entityClass, ".ctor", 1);
			if (!constructor) { LogError("Failed to Get the Entity Constructor Method, ID: {}", eID); return nullptr; }

			void* param = &eID;
			MonoObject* exc = nullptr;
			mono_runtime_invoke(constructor, entity, &param, &exc);

			if (exc)
				Scripting::MonoUtils::PrintExceptionDetails(exc);

			data.entities[eID] = entity;

		}

		return data.entities[eID];

	}

	std::vector<std::string> GetScriptComponents() { return data.scriptComponents; }
	std::vector<ScriptField> GetScriptFields(const std::string& scriptName) { return data.scriptFields[scriptName]; }

	MonoClass* GetVector2MonoClass() { return data.vector2Class; }
	MonoClass* GetVector3MonoClass() { return data.vector3Class; }

	MonoClass* GetEntityMonoClass() { return data.entityClass; }

	MonoDomain* GetRootDomain() { return data.root; }
	MonoDomain* GetAppDomain() { return data.app; }

}