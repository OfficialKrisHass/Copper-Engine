#include "cupch.h"
#include "ScriptingCore.h"

#include "Engine/Utilities/FileUtils.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/InternalCalls.cpp"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

char domainName[] = "CUScriptRuntime";

namespace Copper::Scripting {

	struct ScriptingCoreData {

		MonoDomain* root;
		MonoDomain* app;

		MonoAssembly* apiAssembly;
		MonoImage* apiAssemblyImage;

		Filesystem::Path projectPath;
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

	void SetupInternalCalls();

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
		SetupInternalCalls();

	}
	void Shutdown() {

		data.app = nullptr;
		data.root = nullptr;

	}

	bool LoadProjectAssembly(const Filesystem::Path& path) {

		data.projectPath = path;

		data.projectAssembly = MonoUtils::LoadAssembly(path);
		if (!data.projectAssembly)
			return false;

		data.projectAssemblyImage = mono_assembly_get_image(data.projectAssembly);

		return true;


	}
	bool Reload(const Filesystem::Path& path, bool initScriptComponents) {

		if (path != "") data.projectPath = path;

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

	void SetupInternalCalls() {
		
		//======== Logging ========

		mono_add_internal_call("Copper.InternalCalls::Log", (void*) InternalCalls::EditorLog);
		mono_add_internal_call("Copper.InternalCalls::LogWarn", (void*) InternalCalls::EditorLogWarn);
		mono_add_internal_call("Copper.InternalCalls::LogError", (void*) InternalCalls::EditorLogError);

		//======== Input ========

		mono_add_internal_call("Copper.InternalCalls::IsKey", (void*) InternalCalls::IsKey);
		mono_add_internal_call("Copper.InternalCalls::IsKeyDown", (void*) InternalCalls::IsKeyDown);
		mono_add_internal_call("Copper.InternalCalls::IsKeyReleased", (void*) InternalCalls::IsKeyReleased);

		mono_add_internal_call("Copper.InternalCalls::GetAxis", (void*) InternalCalls::GetAxis);

		mono_add_internal_call("Copper.InternalCalls::SetCursorVisible", (void*) InternalCalls::SetCursorVisible);
		mono_add_internal_call("Copper.InternalCalls::SetCursorLocked", (void*) InternalCalls::SetCursorLocked);

		//======== Object ========

		mono_add_internal_call("Copper.InternalCalls::GetEntityName", (void*) InternalCalls::GetEntityName);
		mono_add_internal_call("Copper.InternalCalls::SetEntityName", (void*) InternalCalls::SetEntityName);

		mono_add_internal_call("Copper.InternalCalls::IsEntityValid", (void*) InternalCalls::IsEntityValid);

		mono_add_internal_call("Copper.InternalCalls::GetEntity", (void*) InternalCalls::GetEntity);

		//======== Components ========

		mono_add_internal_call("Copper.InternalCalls::AddComponent", (void*) InternalCalls::AddComponent);
		mono_add_internal_call("Copper.InternalCalls::GetComponent", (void*) InternalCalls::GetComponent);
		mono_add_internal_call("Copper.InternalCalls::HasComponent", (void*) InternalCalls::HasComponent);

		mono_add_internal_call("Copper.InternalCalls::SetComponentEID", (void*) InternalCalls::SetComponentObjID);

		//======== Transform ========

		mono_add_internal_call("Copper.InternalCalls::GetPosition", (void*) InternalCalls::GetPosition);
		mono_add_internal_call("Copper.InternalCalls::GetRotation", (void*) InternalCalls::GetRotation);
		mono_add_internal_call("Copper.InternalCalls::GetScale", (void*) InternalCalls::GetScale);
		mono_add_internal_call("Copper.InternalCalls::SetPosition", (void*) InternalCalls::SetPosition);
		mono_add_internal_call("Copper.InternalCalls::SetRotation", (void*) InternalCalls::SetRotation);
		mono_add_internal_call("Copper.InternalCalls::SetScale", (void*) InternalCalls::SetScale);

		mono_add_internal_call("Copper.InternalCalls::GetForward", (void*) InternalCalls::GetForward);
		mono_add_internal_call("Copper.InternalCalls::GetRight", (void*) InternalCalls::GetRight);
		mono_add_internal_call("Copper.InternalCalls::GetUp",    (void*) InternalCalls::GetUp);

		//======== Camera ========

		mono_add_internal_call("Copper.InternalCalls::CameraGetFOV", (void*) InternalCalls::CameraGetFOV);
		mono_add_internal_call("Copper.InternalCalls::CameraGetNearPlane", (void*) InternalCalls::CameraGetNearPlane);
		mono_add_internal_call("Copper.InternalCalls::CameraGetFarPlane", (void*) InternalCalls::CameraGetFarPlane);
		mono_add_internal_call("Copper.InternalCalls::CameraSetFOV", (void*) InternalCalls::CameraSetFOV);
		mono_add_internal_call("Copper.InternalCalls::CameraSetNearPlane", (void*) InternalCalls::CameraSetNearPlane);
		mono_add_internal_call("Copper.InternalCalls::CameraSetFarPlane", (void*) InternalCalls::CameraSetFarPlane);

		//======== RigidBody ========

		mono_add_internal_call("Copper.InternalCalls::RigidBodyGetIsStatic", (void*) InternalCalls::RigidBodyGetIsStatic);
		mono_add_internal_call("Copper.InternalCalls::RigidBodyGetGravity", (void*) InternalCalls::RigidBodyGetGravity);
		mono_add_internal_call("Copper.InternalCalls::RigidBodySetIsStatic", (void*) InternalCalls::RigidBodySetIsStatic);
		mono_add_internal_call("Copper.InternalCalls::RigidBodySetGravity", (void*) InternalCalls::RigidBodySetGravity);

		mono_add_internal_call("Copper.InternalCalls::RigidBodyGetMass", (void*) InternalCalls::RigidBodyGetMass);
		mono_add_internal_call("Copper.InternalCalls::RigidBodySetMass", (void*) InternalCalls::RigidBodySetMass);

		//======== BoxCollider ========

		mono_add_internal_call("Copper.InternalCalls::BoxColliderGetTrigger", (void*) InternalCalls::BoxColliderGetTrigger);
		mono_add_internal_call("Copper.InternalCalls::BoxColliderSetTrigger", (void*) InternalCalls::BoxColliderSetTrigger);

		mono_add_internal_call("Copper.InternalCalls::BoxColliderGetCenter", (void*) InternalCalls::BoxColliderGetCenter);
		mono_add_internal_call("Copper.InternalCalls::BoxColliderGetSize", (void*) InternalCalls::BoxColliderGetSize);
		mono_add_internal_call("Copper.InternalCalls::BoxColliderSetCenter", (void*) InternalCalls::BoxColliderSetCenter);
		mono_add_internal_call("Copper.InternalCalls::BoxColliderSetSize", (void*) InternalCalls::BoxColliderSetSize);

		//======== Quaternion ========

		mono_add_internal_call("Copper.InternalCalls::QuaternionEulerAngles", (void*) InternalCalls::QuaternionEulerAngles);

	}

	std::vector<std::string> GetScriptComponents() { return data.scriptComponents; }
	std::vector<ScriptField> GetScriptFields(const std::string& scriptName) { return data.scriptFields[scriptName]; }

	MonoClass* GetVector2MonoClass() { return data.vector2Class; }
	MonoClass* GetVector3MonoClass() { return data.vector3Class; }

	MonoClass* GetEntityMonoClass() { return data.entityClass; }

	MonoDomain* GetRootDomain() { return data.root; }
	MonoDomain* GetAppDomain() { return data.app; }

}