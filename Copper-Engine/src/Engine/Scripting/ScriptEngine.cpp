#include "cupch.h"
#include "ScriptEngine.h"

#include "Engine/Core/Engine.h"

#include "Engine/Components/Light.h"
#include "Engine/Components/MeshRenderer.h"

#include <CopperECS/CopperECS.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/attrdefs.h>

namespace Copper::ScriptEngine {

	namespace Utils {

		enum class Accessibility : uint8_t {

			None = 0,
			Public = (1 << 0),
			Private = (1 << 1)

		};

		bool CheckMonoError(MonoError* error) {

			bool hasError = !mono_error_ok(error);
			if (hasError) {

				unsigned short errorCode = mono_error_get_error_code(error);
				const char* errorMessage = mono_error_get_message(error);

				LogError("Mono Error!\nCode: {0}\nError\n\n{1}", errorCode, errorMessage);

				mono_error_cleanup(error);

			}

			return hasError;

		}

		std::string MonoStringToString(MonoString* string) {

			if (string == nullptr || mono_string_length(string) == 0) { LogError("MonoString is nullptr or empty"); return ""; }

			MonoError error;
			char* utf8 = mono_string_to_utf8_checked(string, &error);
			if (CheckMonoError(&error)) return "";

			std::string ret(utf8);
			mono_free(utf8);

			return ret;

		}
		MonoString* StringToMonoString(std::string string, MonoDomain* domain) {

			if (string.size() == 0) { LogError("String is nullptr or empty!"); return nullptr; }

			MonoString* ret = mono_string_new(domain, string.c_str());

			return ret;

		}

		std::string VariableTypeToString(VariableType type) {

			switch (type) {

				case VariableType::Int: { return "Int"; break; }
				case VariableType::UInt: { return "Unsigned Int"; break; }
				case VariableType::Float: { return "Float"; break; }
				case VariableType::Double: { return "Double"; break; }
				case VariableType::String: { return "String"; break; }
				case VariableType::Char: { return "Char"; break; }
				case VariableType::Vector2: { return "Vector2"; break; }
				case VariableType::Vector3: { return "Vector3"; break; }

			}

			return "";

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
		MonoAssembly* LoadAssembly(std::filesystem::path path) {

			uint32_t fileSize = 0;
			char* fileData = Utils::ReadAssembly(path.string(), &fileSize);

			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK) { LogError("Failed to Read Assembly.\n\n{0}", mono_image_strerror(status)); return nullptr; }

			MonoAssembly* assembly = mono_assembly_load_from_full(image, path.string().c_str(), &status, 0);
			mono_image_close(image);

			//Cleanup
			delete[] fileData;

			return assembly;

		}

		void LogAssemblyClasses(MonoAssembly* assembly) {

			MonoImage* image = mono_assembly_get_image(assembly);
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
		void LogClassFieldsAndProperties(MonoClass* klass) {

			void* iter = nullptr;
			MonoClassField* field;
			while (field = mono_class_get_fields(klass, &iter)) {

				Log("Field: {0}", mono_field_get_name(field));

			}

			iter = nullptr;
			MonoProperty* property;
			while (property = mono_class_get_properties(klass, &iter)) {

				Log("Property: {0}", mono_property_get_name(property));

			}

		}

		uint8_t GetFieldAccssibility(MonoClassField* field) {

			uint8_t ret = (uint8_t) Accessibility::None;
			uint32_t flag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

			switch (flag) {

				case MONO_FIELD_ATTR_PRIVATE:
				{

					ret = (uint8_t) Accessibility::Private;
					break;

				}
				case MONO_FIELD_ATTR_PUBLIC:
				{

					ret = (uint8_t) Accessibility::Public;
					break;

				}
			}

			return ret;

		}
		uint8_t GetPropertyAccessibility(MonoProperty* property) {

			uint8_t ret = (uint8_t) Accessibility::None;

			MonoMethod* getter = mono_property_get_get_method(property);
			if (getter != nullptr) {

				uint32_t flag = mono_method_get_flags(getter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;

				switch (flag) {

					case MONO_FIELD_ATTR_PRIVATE:
					{

						ret = (uint8_t) Accessibility::Private;
						break;

					}
					case MONO_FIELD_ATTR_PUBLIC:
					{

						ret = (uint8_t) Accessibility::Public;
						break;

					}

				}

			}

			MonoMethod* setter = mono_property_get_set_method(property);
			if (setter != nullptr) {

				uint32_t accessFlag = mono_method_get_flags(setter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;
				if (accessFlag != MONO_FIELD_ATTR_PUBLIC) ret = (uint8_t) Accessibility::Private;

			} else {

				ret = (uint8_t) Accessibility::Private;

			}

			return ret;

		}

	}

	struct ScriptEngineData {

		//Domains
		MonoDomain* root;
		MonoDomain* app;

		//Scripting API Assembly
		MonoAssembly* APIAssembly;
		MonoImage* APIAssemblyImage;

		//The Base Component Class Reference
		MonoClass* componentClass;

		//List of the names of all Script Components
		std::vector<ScriptComponent> scriptComponents;
		std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Variable>>> scriptFieldAndProperties;

	};

	ScriptEngineData data;

	namespace InternalFunctions {

		static std::unordered_map<MonoType*, std::function<bool(Object)>> componentHasFunctions;
		static std::unordered_map<MonoType*, std::function<bool(Object, std::string, std::string)>> scriptComponentHasFunctions;

		template<typename T> static void RegisterCoreComponent() {

			std::string typeName = typeid(T).name();
			std::string name = fmt::format("Copper.{}", typeName.substr(typeName.find_last_of(':') + 1));

			MonoType* type = mono_reflection_type_from_name(name.data(), ScriptEngine::GetAssemblyImage());
			if (!type) { LogError("Could not find Component {0}", name); return; }

			componentHasFunctions[type] = [](Object obj) { return obj.HasComponent<T>(); };

		}
		static void RegisterScriptComponent(std::string nameSpace, std::string name) {

			std::string fullName = fmt::format("{}.{}", nameSpace, name);
			MonoType* type = mono_reflection_type_from_name(fullName.data(), ScriptEngine::GetAssemblyImage());
			if (!type) { LogError("Could not find Component {0}", name); return; }

			scriptComponentHasFunctions[type] = [](Object obj, std::string nameSpace, std::string name) { return obj.HasScriptComponent(nameSpace, name); };

		}

		//Logging
		static void InternalLog(MonoString* string) { Log(Utils::MonoStringToString(string)); }
		static void InternalLogWarn(MonoString* string) { LogWarn(Utils::MonoStringToString(string)); }
		static void InternalLogError(MonoString* string) { LogError(Utils::MonoStringToString(string)); }

		//Object
		static void InternalGetObject(int32_t id, Object* out) {

			Scene* scene = GetScene();
			*out = scene->registry.GetObjectFromID(id);

		}

		//Components
		static bool InternalHasComponent(int32_t id, MonoReflectionType* componentType) {

			Object obj = GetScene()->registry.GetObjectFromID(id);
			MonoType* managedType = mono_reflection_type_get_type(componentType);

			bool scriptComponent = scriptComponentHasFunctions.find(managedType) != scriptComponentHasFunctions.end();
			if (componentHasFunctions.find(managedType) == componentHasFunctions.end() &&
				!scriptComponent) {

				LogError("HasComponent couldn't find the Component Has Function");

			}

			if (scriptComponent) {

				std::string name = mono_type_get_name(managedType);
				std::string nameSpace = name.substr(0, name.find_last_of('.'));
				std::string scriptName = name.substr(name.find_last_of('.') + 1);

				return scriptComponentHasFunctions.at(managedType)(obj, nameSpace, scriptName);

			}

			return componentHasFunctions.at(managedType)(obj);

		}
		static void InternalGetComponent(int32_t id, MonoReflectionType* cType, MonoObject* out) {

			//This seems primitive but actually I can't find another way on how to do this.
			//This was my first idea and it actually works, however there definitely is a better way
			//But I'm happy for now

			Object obj = GetScene()->registry.GetObjectFromID(id);
			MonoType* type = mono_reflection_type_get_type(cType);

			std::string name = mono_type_get_name(type);
			std::string nameSpace = name.substr(0, name.find_last_of('.'));
			std::string scriptName = name.substr(name.find_last_of('.') + 1);
			ScriptComponent* script = obj.GetScriptComponent(nameSpace, scriptName);

			if (script == nullptr) { LogError("Failed to Get the Script Component {0}.{1}", nameSpace, scriptName); return; }

			std::unordered_map<std::string, std::vector<Variable>> fieldsAndProperties = ScriptEngine::GetScriptFieldsAndProperties(nameSpace, scriptName);
			for (Variable field : fieldsAndProperties["Fields"]) {

				int tmp;
				field.GetValue(script->GetInstance(), &tmp);
				field.SetValue(out, &tmp);

			}

		}

		//Transform
		static void InternalTransformGetPosition(int32_t id, Vector3* out) {

			Scene* scene = GetScene();
			*out = scene->registry.GetObjectFromID(id).transform->position;

		}
		static void InternalTransformSetPosition(int32_t id, Vector3* value) {

			Scene* scene = GetScene();
			scene->registry.GetObjectFromID(id).transform->position = *value;

		}
		static void InternalTransformGetRotation(int32_t id, Vector3* out) {

			Scene* scene = GetScene();
			*out = scene->registry.GetObjectFromID(id).transform->rotation;

		}
		static void InternalTransformSetRotation(int32_t id, Vector3* value) {

			Scene* scene = GetScene();
			scene->registry.GetObjectFromID(id).transform->rotation = *value;

		}
		static void InternalTransformGetScale(int32_t id, Vector3* out) {

			Scene* scene = GetScene();
			*out = scene->registry.GetObjectFromID(id).transform->scale;

		}
		static void InternalTransformSetScale(int32_t id, Vector3* value) {

			Scene* scene = GetScene();
			scene->registry.GetObjectFromID(id).transform->scale = *value;

		}

	}

	void InitMono();
	void ShutdownMono();

	void InitInternalFunctions();

	void RegisterCoreComponents();

	void LoadScriptComponents();
	void LoadScriptFieldAndProperties();

	void Initialize() {

		InitMono();

		//Load Assembly
		data.app = mono_domain_create_appdomain("CopperAppDomain", nullptr);
		mono_domain_set(data.app, true);

		data.APIAssembly = Utils::LoadAssembly("assets/ScriptAPI/Copper-ScriptingAPI.dll");
		data.APIAssemblyImage = mono_assembly_get_image(data.APIAssembly);

		data.componentClass = mono_class_from_name(data.APIAssemblyImage, "Copper", "Component");

		InitInternalFunctions();

		RegisterCoreComponents();

		LoadScriptComponents();
		LoadScriptFieldAndProperties();

	}
	void Shutdown() {

		ShutdownMono();

	}

	void InitMono() {

		mono_set_assemblies_path("lib/mono/lib");

		data.root = mono_jit_init("CopperJitRuntime");
		if (!data.root) { LogError("Mono Failed to Initialize Root Domain"); return; }

	}
	void ShutdownMono() {

		//

	}

	void RegisterCoreComponents() {

		InternalFunctions::RegisterCoreComponent<Transform>();
		InternalFunctions::RegisterCoreComponent<Camera>();
		InternalFunctions::RegisterCoreComponent<Light>();
		InternalFunctions::RegisterCoreComponent<MeshRenderer>();

	}

	void LoadScriptComponents() {

		data.scriptComponents.clear();

		MonoImage* image = mono_assembly_get_image(data.APIAssembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numOfTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int i = 0; i < numOfTypes; i++) {

			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			if (std::string(name) == "<Module>") continue;
			if (std::string(name) == "Component") continue;

			MonoClass* script = mono_class_from_name(image, nameSpace, name);

			if (mono_class_is_subclass_of(script, data.componentClass, false)) {

				data.scriptComponents.push_back(ScriptComponent(nameSpace, name, script));
				InternalFunctions::RegisterScriptComponent(nameSpace, name);

			}

		}

	}
	void LoadScriptFieldAndProperties() {

		for (ScriptComponent script : data.scriptComponents) {

			std::unordered_map<std::string, std::vector<Variable>> fieldAndProperties;

			fieldAndProperties["Fields"] = std::vector<Variable>();
			fieldAndProperties["Properties"] = std::vector<Variable>();

			MonoImage* image = mono_assembly_get_image(data.APIAssembly);
			MonoClass* scriptClass = mono_class_from_name(image, script.nameSpace.c_str(), script.scriptName.c_str());

			void* iter = nullptr;
			MonoClassField* field;
			while (field = mono_class_get_fields(scriptClass, &iter)) {

				if (Utils::GetFieldAccssibility(field) != (uint8_t) Utils::Accessibility::Public) continue;

				fieldAndProperties["Fields"].push_back(Variable());
				fieldAndProperties["Fields"].back().type = Type::Field;
				fieldAndProperties["Fields"].back().name = mono_field_get_name(field);

				MonoType* type = mono_field_get_type(field);
				std::string typeName = mono_type_get_name(type);

				VariableType varType = VariableType::None;
				if (typeName == "System.Int32") varType = VariableType::Int;
				else if (typeName == "System.UInt32") varType = VariableType::UInt;
				else if (typeName == "System.Single") varType = VariableType::Float;
				else if (typeName == "System.Double") varType = VariableType::Double;
				else if (typeName == "System.String") varType = VariableType::String;
				else if (typeName == "System.Char") varType = VariableType::Char;
				else if (typeName == "Copper.Vector2") varType = VariableType::Vector2;
				else if (typeName == "Copper.Vector3") varType = VariableType::Vector3;

				fieldAndProperties["Fields"].back().varType = varType;
				fieldAndProperties["Fields"].back().script = script.klass;

				//Log("Field {0} Type: {1}", mono_field_get_name(field), mono_type_get_name(type));

			}

			iter = nullptr;
			MonoProperty* property;
			while (property = mono_class_get_properties(scriptClass, &iter)) {

				if (Utils::GetPropertyAccessibility(property) != (uint8_t) Utils::Accessibility::Public) continue;

				fieldAndProperties["Properties"].push_back(Variable());
				fieldAndProperties["Properties"].back().type = Type::Property;
				fieldAndProperties["Properties"].back().name = mono_property_get_name(property);

				MonoMethod* getter = mono_property_get_get_method(property);
				MonoMethodSignature* getterSignature = mono_method_get_signature(getter, 0, 0);
				MonoType* type = mono_signature_get_return_type(getterSignature);
				std::string typeName = mono_type_get_name(type);

				VariableType varType = VariableType::None;
				if (typeName == "System.Int32") varType = VariableType::Int;
				else if (typeName == "System.UInt32") varType = VariableType::UInt;
				else if (typeName == "System.Single") varType = VariableType::Float;
				else if (typeName == "System.Double") varType = VariableType::Double;
				else if (typeName == "System.String") varType = VariableType::String;
				else if (typeName == "System.Char") varType = VariableType::Char;
				else if (typeName == "Copper.Vector2") varType = VariableType::Vector2;
				else if (typeName == "Copper.Vector3") varType = VariableType::Vector3;

				fieldAndProperties["Properties"].back().varType = varType;

				//Log("Property {0} Type: {1}", mono_property_get_name(property), mono_type_get_name(type));

			}

			data.scriptFieldAndProperties[script.nameSpace + '.' + script.scriptName] = fieldAndProperties;

		}

	}
	void InitInternalFunctions() {

		//Logging
		mono_add_internal_call("Copper.InternalFunctions::Log", InternalFunctions::InternalLog);
		mono_add_internal_call("Copper.InternalFunctions::LogWarn", InternalFunctions::InternalLogWarn);
		mono_add_internal_call("Copper.InternalFunctions::LogError", InternalFunctions::InternalLogError);

		//Object
		mono_add_internal_call("Copper.InternalFunctions::GetObject", InternalFunctions::InternalGetObject);

		//Components
		mono_add_internal_call("Copper.InternalFunctions::HasComponent", InternalFunctions::InternalHasComponent);
		mono_add_internal_call("Copper.InternalFunctions::GetComponent", InternalFunctions::InternalGetComponent);

		//Transform
		mono_add_internal_call("Copper.InternalFunctions::TransformGetPosition", InternalFunctions::InternalTransformGetPosition);
		mono_add_internal_call("Copper.InternalFunctions::TransformSetPosition", InternalFunctions::InternalTransformSetPosition);
		mono_add_internal_call("Copper.InternalFunctions::TransformGetRotation", InternalFunctions::InternalTransformGetRotation);
		mono_add_internal_call("Copper.InternalFunctions::TransformSetRotation", InternalFunctions::InternalTransformSetRotation);
		mono_add_internal_call("Copper.InternalFunctions::TransformGetScale", InternalFunctions::InternalTransformGetScale);
		mono_add_internal_call("Copper.InternalFunctions::TransformSetScale", InternalFunctions::InternalTransformSetScale);

	}

	MonoObject* AddScriptComponent(Object& obj, std::string nameSpace, std::string scriptName) {

		MonoImage* image = mono_assembly_get_image(data.APIAssembly);

		std::string classNamespace;
		std::string className;

		//Create a Class
		MonoClass* script = mono_class_from_name(image, nameSpace.c_str(), scriptName.c_str());
		if (!script) { LogError("Failed to Get a Class Reference!"); return nullptr; }

		//Check if Class is a Component
		bool isEntity = mono_class_is_subclass_of(script, data.componentClass, false);
		if (!isEntity) { LogError("Script {0}.{1} isn't a Component!", nameSpace, scriptName); return nullptr; }

		//Instantiate Class
		MonoObject* instance = mono_object_new(data.app, script);
		if (!instance) { LogError("Failed to Create a class Instance!"); return nullptr; }

		//Call the Constructor
		mono_runtime_object_init(instance);

		MonoMethod* method = mono_class_get_method_from_name(data.componentClass, ".ctor", 1);
		if (!method) { LogError("Failed to get Component Constructor!"); return instance; }

		void* param = &obj.id;

		MonoObject* exception = nullptr;
		mono_runtime_invoke(method, instance, &param , &exception);

		return instance;

	}

	MonoImage* GetAssemblyImage() { return mono_assembly_get_image(data.APIAssembly); }

	std::vector<ScriptComponent> GetScriptComponents() { return data.scriptComponents; }
	std::unordered_map<std::string, std::vector<Variable>> GetScriptFieldsAndProperties(std::string nameSpace, std::string name) { return data.scriptFieldAndProperties[nameSpace + '.' + name]; }

}

namespace Copper {

	void Variable::GetValue(MonoObject* instance, void* out) {

		switch (type) {

			case Type::Field: {

				MonoClassField* field = mono_class_get_field_from_name(script, name.c_str());
				mono_field_get_value(instance, field, out);

				break;

			}
			case Type::Property: {

				MonoProperty* property = mono_class_get_property_from_name(script, name.c_str());
				out = mono_property_get_value(property, instance, nullptr, nullptr);

			}

		}

	}
	void Variable::SetValue(MonoObject* instance, void* value) {

		switch (type) {

			case Type::Field: {

				MonoClassField* field = mono_class_get_field_from_name(script, name.c_str());
				mono_field_set_value(instance, field, value);
				break;

			}
			case Type::Property: {

				MonoProperty* property = mono_class_get_property_from_name(script, name.c_str());
				mono_property_set_value(property, instance, &value, nullptr);
				break;

			}

		}

	}

}