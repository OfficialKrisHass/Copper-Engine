#include "cupch.h"
#include "Scene.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/CopperECS.h"
#include "Engine/Scene/OldSceneDeserialization.h"

#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Light.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Components/RigidBody.h"

#include "Engine/Components/Collider.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/SphereCollider.h"
#include "Engine/Components/CapsuleCollider.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Primitives.h"

#include "Engine/AssetStorage/AssetMap.h"
#include "Engine/AssetStorage/AssetStorage.h"

#include "Engine/Physics/Raycast.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Engine/YAMLOverloads/Everything.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Copper {

	namespace Renderer {

		extern void StartFrame();

		extern void Render(Camera* cam, bool gizmos);

	}

	uint32 cCounter = 0;

	std::unordered_map<uint32, std::function<bool(const YAML::Node&, Scene*)>> oldDeserializeFunctions;

	void Scene::StartRuntime() {

		CUP_FUNCTION();

		m_runtimeRunning = true;

		InitializePhysics();

		for (InternalEntity* entity : ComponentViewOR<RigidBody, BoxCollider>(this)) {

			if (RigidBody* rb = entity->GetComponent<RigidBody>()) rb->Setup();
			else if (BoxCollider* collider = entity->GetComponent<BoxCollider>()) collider->Setup();

		}

	}
	void Scene::StopRuntime() {

		CUP_FUNCTION();

		Renderer::ClearLights();

	}
	void Scene::Update(float deltaTime) {

		CUP_FUNCTION();

		Renderer::StartFrame();

		// Physics

		if (m_runtimeRunning) UpdatePhysics(deltaTime);

		CUP_START_FRAME("ECS Update");

		for (InternalEntity* entity : EntityView(this)) {

			if (m_runtimeRunning)
				RuntimeUpdateEntity(entity, deltaTime);

			entity->m_transform->Update();

			if (Light* lightComponent = entity->GetComponent<Light>()) {
				
				Renderer::AddLight(lightComponent);
				Renderer::AddLine(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Color::green, entity->m_transform);
			
			}
			if (Camera* cameraComponent = entity->GetComponent<Camera>()) {

				Renderer::SetCamera(cameraComponent);
				cam = cameraComponent;

				if (m_runtimeRunning) {

					Raycast::Data hitData;
					if (Raycast::Fire(entity->m_transform->m_globalPosition, entity->m_transform->m_forward, &hitData)) 
						Renderer::AddLine(hitData.position, hitData.position + hitData.normal, Color::blue);

				}

			}
			if (MeshRenderer* renderer = entity->GetComponent<MeshRenderer>())
				Renderer::AddMesh(renderer->mesh, entity->m_transform, renderer->material);

			if (Collider* collider = entity->GetComponent<Collider>())
				Renderer::AddCube(Vector3::zero, Vector3::one, Color::red, entity->m_transform);

		}
		if (m_runtimeRunning && !m_runtimeStarted) m_runtimeStarted = true;

		CUP_END_FRAME();

		Renderer::RenderBatch();

	}
	void Scene::RuntimeUpdateEntity(InternalEntity* entity, float deltaTIme) {

		CUP_FUNCTION();

		if (RigidBody* rb = entity->GetComponent<RigidBody>())
			rb->UpdatePositionAndRotation();

		if (ScriptComponent* script = entity->GetComponent<ScriptComponent>()) {

			if (!m_runtimeStarted)
				script->InvokeCreate();
			script->InvokeUpdate();

		}
		

	}

	void Scene::Render(Camera* cam, bool gizmos) { CUP_FUNCTION(); Renderer::Render(cam, gizmos); }

	void Scene::Serialize(const fs::path& path) {

		CUP_FUNCTION();

		this->path = path;
		this->path.replace_extension("copper");

		this->name = path.filename().string();

		YAML::Emitter out;
		out << YAML::BeginMap; // Main

		// Scene Info

		out << YAML::Key << "Version" << YAML::Value << SCENE_VERSION;
		out << YAML::Key << "Name" << YAML::Value << name;

		// Entities

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginMap; // Entities
		for (InternalEntity* entity : EntityView(this)) {

			SerializeEntity(entity, out);

		}
		out << YAML::EndMap; // Entities

		out << YAML::EndMap; // Main
		std::ofstream file(this->path);
		file << out.c_str();

	}
	bool Scene::Deserialize(const fs::path& path) {

		CUP_FUNCTION();

		if (m_physicsInitialized) ShutdownPhysics();

		this->path = path;

		m_registry.Cleanup();
		m_registry.Initialize();

		m_runtimeRunning = false;
		m_runtimeStarted = false;
		m_physicsInitialized = false;

		cam = nullptr;

		YAML::Node data;
		try { data = YAML::LoadFile(path.string()); } catch(YAML::ParserException e) {
			
			LogError("Failed to Read .scene file. {}", path.string());
			LogError("    {}", e.what());
			return false;
			
		}

		YAML::Node versionNode = data["Version"];
		if (versionNode.IsSequence()) {

			return OldSceneDeserialization::DeserializeVersion_Beta1_0_0(data, this);

		}

		uint32 sceneVersion = versionNode.as<uint32>();
		if (oldDeserializeFunctions.find(sceneVersion) != oldDeserializeFunctions.end()) return oldDeserializeFunctions[sceneVersion](data, this);
		CU_ASSERT(sceneVersion == SCENE_VERSION, "The Scene you tried to open has an invalid version of {}\n    Path: {}", sceneVersion, path.string());

		this->name = data["Name"].as<std::string>();

		// Entities

		YAML::Node entities = data["Entities"];
		for (YAML::const_iterator it = entities.begin(); it != entities.end(); ++it) {

			std::string name = it->first.as<std::string>();
			uint32 id = it->second["ID"].as<uint32>();
			InternalEntity* entity = CreateEntityFromID(id, Vector3::zero, Quaternion(), Vector3::one, name, false);

			DeserializeEntity(entity, it->second);

		}

		return true;
		
	}

	void Scene::SerializeEntity(InternalEntity* entity, YAML::Emitter& out) {

		CUP_FUNCTION();

		out << YAML::Key << entity->name << YAML::Value << YAML::BeginMap; // Entity

		out << YAML::Key << "ID" << YAML::Value << entity->m_id;
		out << YAML::Key << "Transform" << YAML::Value << YAML::BeginMap; // Transform

		out << YAML::Key << "Position" << YAML::Value << entity->m_transform->m_position;
		out << YAML::Key << "Rotation" << YAML::Value << entity->m_transform->m_rotation;
		out << YAML::Key << "Scale" << YAML::Value << entity->m_transform->m_scale;

		out << YAML::Key << "Parent ID" << YAML::Value;
		if (entity->m_transform->m_parent) out << entity->m_transform->m_parent->m_entity.m_id;
		else out << INVALID_ENTITY_ID;

		out << YAML::Key << "Children" << YAML::Value << YAML::Flow << YAML::BeginSeq; // Children
		for (uint32 childID : entity->m_transform->m_children) {

			out << childID;

		}
		out << YAML::EndSeq; // Children

		out << YAML::EndMap; // Transform

		if (MeshRenderer* renderer = entity->GetComponent<MeshRenderer>()) {

			out << YAML::Key << "Mesh Renderer" << YAML::Value << YAML::BeginMap; // Mesh Renderer

			out << YAML::Key << "Mesh" << YAML::Value << renderer->mesh;
			out << YAML::Key << "Material" << YAML::Value << renderer->material;

			out << YAML::EndMap; // Mesh Renderer

		}
		if (Light* light = entity->GetComponent<Light>()) {

			out << YAML::Key << "Light" << YAML::Value << YAML::BeginMap; // Light

			out << YAML::Key << "Type" << YAML::Value << (uint32) light->type;

			out << YAML::Key << "Color" << YAML::Value << light->color;
			out << YAML::Key << "Intensity" << YAML::Value << light->intensity;

			out << YAML::EndMap; // Light

		}
		if (Camera* camera = entity->GetComponent<Camera>()) {

			out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap; // Camera

			out << YAML::Key << "Fov" << YAML::Value << camera->fov;
			out << YAML::Key << "Near Plane" << YAML::Value << camera->nearPlane;
			out << YAML::Key << "Far Plane" << YAML::Value << camera->farPlane;

			out << YAML::Key << "Size" << YAML::Value << camera->size;

			out << YAML::EndMap; // Camera

		}

		if (RigidBody* rb = entity->GetComponent<RigidBody>()) {

			out << YAML::Key << "Rigid Body" << YAML::Value << YAML::BeginMap; // Rigid Body

			out << YAML::Key << "Static" << YAML::Value << rb->isStatic;
			out << YAML::Key << "Gravity" << YAML::Value << rb->gravity;

			out << YAML::Key << "Mass" << YAML::Value << rb->mass;

			out << YAML::Key << "Lock Mask" << YAML::Value << (uint32) rb->m_lockMask;

			out << YAML::EndMap; // Rigid Body

		}

		if (BoxCollider* collider = entity->GetComponent<BoxCollider>()) {

			out << YAML::Key << "Box Collider" << YAML::Value << YAML::BeginMap; // Box Collider

			out << YAML::Key << "Trigger" << YAML::Value << collider->trigger;
			out << YAML::Key << "Center" << YAML::Value << collider->center;

			out << YAML::Key << "Size" << YAML::Value << collider->size;

			out << YAML::EndMap; // Box Collider

		}
		if (SphereCollider* collider = entity->GetComponent<SphereCollider>()) {

			out << YAML::Key << "Sphere Collider" << YAML::Value << YAML::BeginMap; // Sphere Collider

			out << YAML::Key << "Trigger" << YAML::Value << collider->trigger;
			out << YAML::Key << "Center" << YAML::Value << collider->center;

			out << YAML::Key << "Radius" << YAML::Value << collider->radius;

			out << YAML::EndMap; // Sphere Collider

		}
		if (CapsuleCollider* collider = entity->GetComponent<CapsuleCollider>()) {

			out << YAML::Key << "Capsule Collider" << YAML::Value << YAML::BeginMap; // Sphere Collider

			out << YAML::Key << "Trigger" << YAML::Value << collider->trigger;
			out << YAML::Key << "Center" << YAML::Value << collider->center;

			out << YAML::Key << "Radius" << YAML::Value << collider->radius;
			out << YAML::Key << "Height" << YAML::Value << collider->height;

			out << YAML::EndMap; // Sphere Collider

		}

		if (ScriptComponent* script = entity->GetComponent<ScriptComponent>()) {

			out << YAML::Key << "Script Component" << YAML::Value << YAML::BeginMap; // Script Component

			out << YAML::Key << "Name" << YAML::Value << script->name;
			out << YAML::Key << "Fields" << YAML::Value << YAML::BeginMap; // Fields
			for (const ScriptField& field : Scripting::GetScriptFields(script->name)) {

				switch (field.type) {

					case ScriptField::Type::Int: SerializeScriptField<int32>(field, script, out); break;
					case ScriptField::Type::UInt: SerializeScriptField<uint32>(field, script, out); break;
					case ScriptField::Type::Float: SerializeScriptField<float>(field, script, out); break;

					case ScriptField::Type::Vector2: SerializeScriptField<Vector2>(field, script, out); break;
					case ScriptField::Type::Vector3: SerializeScriptField<Vector3>(field, script, out); break;

					case ScriptField::Type::Entity: SerializeScriptField<InternalEntity*>(field, script, out); break;
					case ScriptField::Type::Transform: SerializeScriptField<Transform*>(field, script, out); break;

				}

			}
			out << YAML::EndMap; // Fields

			out << YAML::EndMap; // Script Component

		}

		out << YAML::EndMap; // Enity

	}
	void Scene::DeserializeEntity(InternalEntity* entity, const YAML::Node& node) {

		CUP_FUNCTION();

		try {

		YAML::Node transform = node["Transform"];
		entity->m_transform->SetPosition(transform["Position"].as<Vector3>());
		entity->m_transform->SetRotation(transform["Rotation"].as<Quaternion>());
		entity->m_transform->SetScale(transform["Scale"].as<Vector3>());

		//Set the Parent
		uint32 parentID = transform["Parent ID"].as<uint32>();
		if (parentID == INVALID_ENTITY_ID) entity->m_transform->m_parent = nullptr;
		else if(entity->m_id < parentID) {

			//This little maneuver is gonna cost us 51 miliseconds
			Entity saved = entity;
			InternalEntity* parent = CreateEntityFromID(parentID, Vector3::zero, Quaternion(), Vector3::one, "Empty Parent");
			entity = saved;

			entity->m_transform->m_parent = parent->m_transform;

			parent->m_transform->m_children.push_back(entity->m_id);

		}

		//Set the children
		for (uint32 i = 0; i < transform["Children"].size(); i++) {

			uint32 childID = transform["Children"][i].as<uint32>();
			if (childID < entity->m_id) continue;
			
			entity->m_transform->m_children.push_back(childID);

			Entity saved = entity;
			InternalEntity* child = CreateEntityFromID(childID, Vector3::zero, Quaternion(), Vector3::one, "Empty Child");
			entity = saved;

			child->m_transform->m_parent = entity->m_transform;

		}

		if (YAML::Node rendererNode = node["Mesh Renderer"]) {

			MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();

			renderer->mesh = rendererNode["Mesh"].as<MeshAsset>();
			renderer->material = rendererNode["Material"].as<MaterialAsset>();

		}
		if (YAML::Node lightNode = node["Light"]) {

			Light* light = entity->AddComponent<Light>();

			light->type = (Light::Type) lightNode["Type"].as<uint32>();

			light->color = lightNode["Color"].as<Color>();
			light->intensity = lightNode["Intensity"].as<float>();

		}
		if (YAML::Node camNode = node["Camera"]) {

			Camera* cam = entity->AddComponent<Camera>();

			cam->fov = camNode["Fov"].as<float>();
			cam->nearPlane = camNode["Near Plane"].as<float>();
			cam->farPlane = camNode["Far Plane"].as<float>();

			cam->size = camNode["Size"].as<UVector2I>();

		}

		if (YAML::Node rbNode = node["Rigid Body"]) {

			RigidBody* rb = entity->AddComponent<RigidBody>();

			rb->isStatic = rbNode["Static"].as<bool>();
			rb->gravity = rbNode["Gravity"].as<bool>();

			rb->mass = rbNode["Mass"].as<float>();

			rb->m_lockMask = rbNode["Lock Mask"].as<uint8>();

		}

		if (YAML::Node colliderNode = node["Box Collider"]) {

			BoxCollider* collider = entity->AddComponent<BoxCollider>();

			collider->trigger = colliderNode["Trigger"].as<bool>();
			collider->center = colliderNode["Center"].as<Vector3>();

			collider->size = colliderNode["Size"].as<Vector3>();

		}
		if (YAML::Node colliderNode = node["Sphere Collider"]) {

			SphereCollider* collider = entity->AddComponent<SphereCollider>();

			collider->trigger = colliderNode["Trigger"].as<bool>();
			collider->center = colliderNode["Center"].as<Vector3>();

			collider->radius = colliderNode["Radius"].as<float>();

		}
		if (YAML::Node colliderNode = node["Capsule Collider"]) {

			CapsuleCollider* collider = entity->AddComponent<CapsuleCollider>();

			collider->trigger = colliderNode["Trigger"].as<bool>();
			collider->center = colliderNode["Center"].as<Vector3>();

			collider->radius = colliderNode["Radius"].as<float>();
			collider->height = colliderNode["Height"].as<float>();

		}

		if (YAML::Node scriptNode = node["Script Component"]) {

			ScriptComponent* script = entity->AddComponent<ScriptComponent>();
			script->Init(scriptNode["Name"].as<std::string>());

			YAML::Node fields = scriptNode["Fields"];
			for (const ScriptField& field : Scripting::GetScriptFields(script->name)) {

				switch (field.type) {

					case ScriptField::Type::Int: DeserializeScriptField<int32>(field, script, fields[field.name]["Value"]); break;
					case ScriptField::Type::UInt: DeserializeScriptField<uint32>(field, script, fields[field.name]["Value"]); break;
					case ScriptField::Type::Float: DeserializeScriptField<float>(field, script, fields[field.name]["Value"]); break;

					case ScriptField::Type::Vector2: DeserializeScriptField<Vector2>(field, script, fields[field.name]["Value"]); break;
					case ScriptField::Type::Vector3: DeserializeScriptField<Vector3>(field, script, fields[field.name]["Value"]); break;

					case ScriptField::Type::Entity: DeserializeScriptField<InternalEntity*>(field, script, fields[field.name]["Value"]); break;
					case ScriptField::Type::Transform: DeserializeScriptField<Transform*>(field, script, fields[field.name]["Value"]); break;

				}

			}

		}

		} catch (YAML::Exception e) {

			LogError("Encountered an exception when trying to deserialize entity {}: {}", *entity, e.msg);

		}

	}

	template<typename T> void Scene::SerializeScriptField(const ScriptField& field, ScriptComponent* instance, YAML::Emitter& out) {
		
		CUP_FUNCTION();

		T value;
		instance->GetFieldValue(field, &value);

		out << YAML::Key << field.name << YAML::Value << YAML::BeginMap; // Field

		out << YAML::Key << "Type" << YAML::Value << (int32) field.type;
		out << YAML::Key << "Value" << YAML::Value << value;

		out << YAML::EndMap; // Field

	}
	template<typename T> void Scene::DeserializeScriptField(const ScriptField& field, ScriptComponent* instance, const YAML::Node& fieldNode) {

		CUP_FUNCTION();

		T tmp = fieldNode.as<T>();
		instance->SetFieldValue(field, &tmp);

	}

	template<> void Scene::SerializeScriptField<Transform*>(const ScriptField& field, ScriptComponent* instance, YAML::Emitter& out) {

		CUP_FUNCTION();

		Transform* value;
		instance->GetFieldValue(field, &value);

		out << YAML::Key << field.name << YAML::Value << YAML::BeginMap; // Field

		out << YAML::Key << "Type" << YAML::Value << (int32) field.type;
		out << YAML::Key << "Value" << YAML::Value << (value ? value->m_entity.m_id : INVALID_ENTITY_ID);

		out << YAML::EndMap; // Field

	}
	template<> void Scene::DeserializeScriptField<Transform*>(const ScriptField& field, ScriptComponent* instance, const YAML::Node& fieldNode) {

		CUP_FUNCTION();

		uint32 eID = fieldNode.as<uint32>();
		Transform* transform = eID == INVALID_ENTITY_ID ? nullptr : GetEntityFromID(eID)->m_transform;
		instance->SetFieldValue(field, &transform);

	}

}