#include "cupch.h"
#include "ScriptingCore.h"

#include "Engine/Utilities/FileUtils.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/InternalCalls.cpp"

//#include "Engine/Scene/Registry.h"

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

		MonoClass* entityClass;
		MonoClass* componentClass;
		MonoClass* vector2Class;
		MonoClass* vector3Class;

		std::vector<std::string> scriptComponents;
		std::unordered_map<std::string, std::vector<ScriptField>> scriptFields;

	};

	ScriptingCoreData data;

	void SetupInternalCalls();

	void InitScriptComponents();
	void InitScriptFields(const std::string& fullName, MonoClass* scriptClass);

	void Initialize() {

		CHECK((GetEngineState() == EngineState::Initialization), "Cannot Initialize Scripting, current Engine State is: {}", EngineStateToString(GetEngineState()));

		//Initialize Mono
		mono_set_assemblies_path("lib/mono/lib");
		data.root = mono_jit_init("CUJITRuntime");

		//Load the ScriptingAPI
		data.app = mono_domain_create_appdomain("CUScriptRuntime", nullptr);
		mono_domain_set(data.app, true);

		data.apiAssembly = MonoUtils::LoadAssembly("assets/ScriptAPI/Copper-ScriptingAPI.dll");
		data.apiAssemblyImage = mono_assembly_get_image(data.apiAssembly);

		data.componentClass = mono_class_from_name(data.apiAssemblyImage, "Copper", "Component");
		data.vector2Class = mono_class_from_name(data.apiAssemblyImage, "Copper", "Vector2");
		data.vector3Class = mono_class_from_name(data.apiAssemblyImage, "Copper", "Vector3");
		data.entityClass = mono_class_from_name(data.apiAssemblyImage, "Copper", "Entity");

		//Setup ScriptingAPI
		SetupInternalCalls();

	}
	void Shutdown() {

		data.app = nullptr;
		data.root = nullptr;

	}

	void LoadProjectAssembly(const std::filesystem::path& path) {

		data.projectPath = path;
		data.projectAssembly = MonoUtils::LoadAssembly(path);
		data.projectAssemblyImage = mono_assembly_get_image(data.projectAssembly);

	}
	void Reload(const std::filesystem::path& path, bool initScriptComponents) {

		if (path != "") data.projectPath = path;

		mono_domain_set(mono_get_root_domain(), false);
		mono_domain_unload(data.app);

		//Load the Scripting API
		data.app = mono_domain_create_appdomain("CUScriptRuntime", nullptr);
		mono_domain_set(data.app, true);

		data.apiAssembly = MonoUtils::LoadAssembly("assets/ScriptAPI/Copper-ScriptingAPI.dll");
		data.apiAssemblyImage = mono_assembly_get_image(data.apiAssembly);

		data.componentClass = mono_class_from_name(data.apiAssemblyImage, "Copper", "Component");
		data.vector2Class = mono_class_from_name(data.apiAssemblyImage, "Copper", "Vector2");
		data.vector3Class = mono_class_from_name(data.apiAssemblyImage, "Copper", "Vector3");
		data.entityClass = mono_class_from_name(data.apiAssemblyImage, "Copper", "Entity");

		//Load the Project Assembly
		LoadProjectAssembly(data.projectPath);

		InitScriptComponents();
		InternalCalls::Initialize();

		if (!initScriptComponents) return;
		Scene* test = GetScene();
		for (Component* component : ComponentView<ScriptComponent>(GetScene())) {

			ScriptComponent* script = (ScriptComponent*) component;
			script->Init(script->name);

		}

	}

	void SetupInternalCalls() {
		
		//Logging
		mono_add_internal_call("Copper.InternalCalls::Log", InternalCalls::EditorLog);
		mono_add_internal_call("Copper.InternalCalls::LogWarn", InternalCalls::EditorLogWarn);
		mono_add_internal_call("Copper.InternalCalls::LogError", InternalCalls::EditorLogError);

		//Input
		mono_add_internal_call("Copper.InternalCalls::IsKey", InternalCalls::IsKey);
		mono_add_internal_call("Copper.InternalCalls::IsKeyDown", InternalCalls::IsKeyDown);
		mono_add_internal_call("Copper.InternalCalls::IsKeyReleased", InternalCalls::IsKeyReleased);

		mono_add_internal_call("Copper.InternalCalls::GetAxis", InternalCalls::GetAxis);

		mono_add_internal_call("Copper.InternalCalls::SetCursorVisible", InternalCalls::SetCursorVisible);
		mono_add_internal_call("Copper.InternalCalls::SetCursorLocked", InternalCalls::SetCursorLocked);

		//Object
		mono_add_internal_call("Copper.InternalCalls::GetEntityName", InternalCalls::GetEntityName);
		mono_add_internal_call("Copper.InternalCalls::SetEntityName", InternalCalls::SetEntityName);

		mono_add_internal_call("Copper.InternalCalls::IsEntityValid", InternalCalls::IsEntityValid);

		mono_add_internal_call("Copper.InternalCalls::GetEntity", InternalCalls::GetEntity);

		//Components
		mono_add_internal_call("Copper.InternalCalls::AddComponent", InternalCalls::AddComponent);
		mono_add_internal_call("Copper.InternalCalls::GetComponent", InternalCalls::GetComponent);
		mono_add_internal_call("Copper.InternalCalls::HasComponent", InternalCalls::HasComponent);

		mono_add_internal_call("Copper.InternalCalls::SetComponentEID", InternalCalls::SetComponentObjID);

		//Transform
		mono_add_internal_call("Copper.InternalCalls::GetPosition", InternalCalls::GetPosition);
		mono_add_internal_call("Copper.InternalCalls::GetRotation", InternalCalls::GetRotation);
		mono_add_internal_call("Copper.InternalCalls::GetScale", InternalCalls::GetScale);
		mono_add_internal_call("Copper.InternalCalls::SetPosition", InternalCalls::SetPosition);
		mono_add_internal_call("Copper.InternalCalls::SetRotation", InternalCalls::SetRotation);
		mono_add_internal_call("Copper.InternalCalls::SetScale", InternalCalls::SetScale);

		mono_add_internal_call("Copper.InternalCalls::GetForward", InternalCalls::GetForward);
		mono_add_internal_call("Copper.InternalCalls::GetBack",  InternalCalls::GetBack);
		mono_add_internal_call("Copper.InternalCalls::GetRight", InternalCalls::GetRight);
		mono_add_internal_call("Copper.InternalCalls::GetLeft",  InternalCalls::GetLeft);
		mono_add_internal_call("Copper.InternalCalls::GetUp",    InternalCalls::GetUp);
		mono_add_internal_call("Copper.InternalCalls::GetDown",  InternalCalls::GetDown);

		//Camera
		mono_add_internal_call("Copper.InternalCalls::CameraGetFOV", InternalCalls::CameraGetFOV);
		mono_add_internal_call("Copper.InternalCalls::CameraGetNearPlane", InternalCalls::CameraGetNearPlane);
		mono_add_internal_call("Copper.InternalCalls::CameraGetFarPlane", InternalCalls::CameraGetFarPlane);
		mono_add_internal_call("Copper.InternalCalls::CameraSetFOV", InternalCalls::CameraSetFOV);
		mono_add_internal_call("Copper.InternalCalls::CameraSetNearPlane", InternalCalls::CameraSetNearPlane);
		mono_add_internal_call("Copper.InternalCalls::CameraSetFarPlane", InternalCalls::CameraSetFarPlane);


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

		mono_runtime_object_init(instance);

		MonoMethod* componentConstructor = mono_class_get_method_from_name(data.componentClass, ".ctor", 1);
		if (!componentConstructor) { LogError("Failed to get the Component Constructor!"); return nullptr; }

		void* param = &eID;
		MonoObject* exception = nullptr;
		mono_runtime_invoke(componentConstructor, instance, &param, &exception);

		return instance;

	}

	std::vector<std::string> GetScriptComponents() { return data.scriptComponents; }
	std::vector<ScriptField> GetScriptFields(const std::string& scriptName) { return data.scriptFields[scriptName]; }

	MonoClass* GetVector2MonoClass() { return data.vector2Class; }
	MonoClass* GetVector3MonoClass() { return data.vector3Class; }

	MonoClass* GetEntityMonoClass() { return data.entityClass; }

	MonoDomain* GetRootDomain() { return data.root; }
	MonoDomain* GetAppDomain() { return data.app; }

}