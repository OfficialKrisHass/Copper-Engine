#include "cupch.h"
#include "Scene.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/EntityView.h"
#include "Engine/Scene/ComponentView.h"
#include "Engine/Scene/OldSceneDeserialization.h"
#include "Engine/Scene/Component.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/ScriptComponent.h"
#include "Engine/Components/Light.h"
#include "Engine/Components/Collider.h"
#include "Engine/Components/PhysicsBody.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Engine/Physics/CollisionData.h"
#include "Engine/Physics/SphereCollider.h"
#include "Engine/Physics/BoxCollider.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#define PhysicsTimeStep 0.02f

namespace Copper {

	int cCounter = 0;

	std::unordered_map<uint32_t, std::function<bool(const YAML::Node&, Scene*)>> oldDeserializeFunctions;

	void Scene::StartRuntime() {

		runtimeRunning = true;

		for (PhysicsBody* body : ComponentView<PhysicsBody>(this)) {

			Vector3& scale = body->GetTransform()->scale;
			body->IbodyInv = glm::inverse((body->mass / 12) * glm::mat3(glm::vec3(scale.y * scale.y + scale.z * scale.z, 0.0f, 0.0f),
																		glm::vec3(0.0f, scale.x * scale.x + scale.z * scale.z, 0.0f),
																		glm::vec3(0.0f, 0.0f, scale.x * scale.x + scale.y * scale.y)));

		}

	}
	void Scene::Update(bool render, float deltaTime) {

		Renderer::ClearColor(0.18f, 0.18f, 0.18f);

		for (Collider* collider : ComponentView<Collider>(this)) {

			collider->checkedAllCollisions = false;

		}
		for (PhysicsBody* body : ComponentView<PhysicsBody>(this)) {

			body->q = glm::quat(body->GetTransform()->rotation);

		}

		for (InternalEntity* entity : EntityView(this)) {

			entity->transform->Update();

			PhysicsBody* physicsBody = entity->GetComponent<PhysicsBody>();
			if (runtimeRunning && physicsBody) {

				physicsBody->force = Vector3::zero;
				if (!physicsBody->staticBody && physicsBody->gravity) physicsBody->force = Vector3(0.0f, -GravityConstant, 0.0f);

				physicsBody->linearVelocity += physicsBody->force / physicsBody->mass * PhysicsTimeStep;
				physicsBody->angularVelocity += physicsBody->torque * 
					glm::mat3((glm::quat) physicsBody->q) * physicsBody->IbodyInv * glm::transpose(glm::mat3((glm::quat) physicsBody->q)) *
					PhysicsTimeStep;
				//physicsBody->angularVelocity += physicsBody->torque / physicsBody->inertia * PhysicsTimeStep;

				physicsBody->GetTransform()->position += physicsBody->linearVelocity * PhysicsTimeStep;

				physicsBody->q = (Quaternion(0.0f, physicsBody->angularVelocity) * physicsBody->q) * 0.5f;
				physicsBody->GetTransform()->rotation += glm::degrees(glm::eulerAngles((glm::quat) physicsBody->q) * PhysicsTimeStep);

			}

			if (Light* lightComponent = entity->GetComponent<Light>()) light = lightComponent;
			if (Camera* cameraComponent = entity->GetComponent<Camera>()) cam = cameraComponent;
			if (MeshRenderer* renderer = entity->GetComponent<MeshRenderer>()) {

				for (Mesh mesh : renderer->meshes) {

					Renderer::AddMesh(&mesh, entity->transform);

				}

			}

			if (!runtimeRunning) continue;

			/*std::vector<CollisionData> collisions;
			if (Collider* collider = entity->GetComponent<Collider>()) {

				for (Collider* other : ComponentView<Collider>(this)) {

					if (other == collider) continue;
					if (other->checkedAllCollisions) continue;

					CollisionData data;
					if (data = collider->Intersects(other)) continue;

					collisions.push_back(data);

				}

				collider->checkedAllCollisions = true;

			}*/

			//for (const CollisionData& data : collisions) {

			//	PhysicsBody* body;
			//	if (!(body = data.a->GetEntity()->GetComponent<PhysicsBody>())) continue;

			//	body->linearVelocity = -body->linearVelocity.Normalize() * data.distance;
			//	body->GetTransform()->position += body->linearVelocity * PhysicsTimeStep;
			//	body->linearVelocity = Vector3::zero;
			//	//body->shouldGravity = false;

			//}

			if (ScriptComponent* script = entity->GetComponent<ScriptComponent>()) {

				if (!runtimeStarted) script->InvokeCreate();
				script->InvokeUpdate();

			}

		}
		if (runtimeRunning && !runtimeStarted) runtimeStarted = true;

		if (cam && render) Render(cam);

	}
	void Scene::Render(Camera* camera) {

		Renderer::RenderFrame(camera, light);
		//Log(camera->size);

	}

	void Scene::Serialize(const std::filesystem::path& path) {

		this->path = path;
		this->name = path.stem().string();

		YAML::Emitter out;
		out << YAML::BeginMap; // Main

		//Version
		out << YAML::Key << "Version" << YAML::Value << SCENE_VERSION;

		//Name
		out << YAML::Key << "Name" << YAML::Value << name;

		//Entities
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginMap; // Entities
		for (InternalEntity* entity : EntityView(this)) {

			SerializeEntity(entity, out);

		}
		out << YAML::EndMap; // Entities

		out << YAML::EndMap; // Main
		std::ofstream file(path);
		file << out.c_str();

	}
	bool Scene::Deserialize(const std::filesystem::path& path) {

		this->path = path;

		registry.Cleanup();

		runtimeRunning = false;
		runtimeStarted = false;
		light = nullptr;
		cam = nullptr;

		YAML::Node data;
		try { data = YAML::LoadFile(path.string()); } catch(YAML::ParserException e) {

			LogError("Failed to Read .scene file. {}\n    {}", path.string(), e.what());
			return false;
			
		}

		YAML::Node versionNode = data["Version"];
		if (versionNode.IsSequence()) {

			return OldSceneDeserialization::DeserializeVersion_Beta1_0_0(data, this);

		}

		uint32_t sceneVersion = versionNode.as<uint32_t>();
		if (oldDeserializeFunctions.find(sceneVersion) != oldDeserializeFunctions.end()) return oldDeserializeFunctions[sceneVersion](data, this);
		CU_ASSERT(sceneVersion == GetVersion().sceneVersion, "The Scene you tried to open has an invalid version ({})\n{}", sceneVersion, path.string());

		this->name = data["Name"].as<std::string>();

		YAML::Node entities = data["Entities"];
		for (YAML::const_iterator it = entities.begin(); it != entities.end(); ++it) {

			std::string name = it->first.as<std::string>();
			uint32_t id = it->second["ID"].as<uint32_t>();
			InternalEntity* entity = CreateEntityFromID(id, name, Vector3::zero, Vector3::zero, Vector3::one, false);

			DeserializeEntity(entity, it->second);

		}

		return true;
		
	}

	void Scene::SerializeEntity(InternalEntity* entity, YAML::Emitter& out) {

		out << YAML::Key << entity->name << YAML::Value << YAML::BeginMap; // Entity

		out << YAML::Key << "ID" << YAML::Value << entity->id;
		out << YAML::Key << "Transform" << YAML::Value << YAML::BeginMap; // Transform

		out << YAML::Key << "Position" << YAML::Value << entity->transform->position;
		out << YAML::Key << "Rotation" << YAML::Value << entity->transform->rotation;
		out << YAML::Key << "Scale" << YAML::Value << entity->transform->scale;

		out << YAML::Key << "Parent ID" << YAML::Value;
		if (entity->transform->parent) out << entity->transform->parent->entity.id;
		else out << invalidID;

		out << YAML::Key << "Children" << YAML::Value << YAML::Flow << YAML::BeginSeq; // Children
		for (uint32_t childID : entity->transform->children) {

			out << childID;

		}
		out << YAML::EndSeq; // Children

		out << YAML::EndMap; // Transform

		if (MeshRenderer* renderer = entity->GetComponent<MeshRenderer>()) {

			out << YAML::Key << "Mesh Renderer" << YAML::Value << YAML::BeginMap; // Mesh Renderer

			for (Mesh& mesh : renderer->meshes) {

				out << YAML::Key << "Mesh" << YAML::Value << YAML::BeginMap; // Mesh

				out << YAML::Key << "Data" << YAML::Value << YAML::BeginSeq; // Data
				for (int i = 0; i < mesh.vertices.size(); i++) {

					out << YAML::BeginMap; // Vertex;

					out << YAML::Key << "Position" << YAML::Value << mesh.vertices[i];
					out << YAML::Key << "Normal" << YAML::Value << mesh.normals[i];
					out << YAML::Key << "Color" << YAML::Value << mesh.colors[i];

					out << YAML::EndMap; // Vertex

				}
				out << YAML::EndSeq; // Data

				out << YAML::Key << "Indices" << YAML::Value << YAML::BeginSeq; // Indices
				for (uint32_t index : mesh.indices)
					out << index;
				out << YAML::EndSeq; // Indices

				out << YAML::EndMap; // Mesh

			}

			out << YAML::EndMap; // Mesh Renderer

		}
		if (Light* light = entity->GetComponent<Light>()) {

			out << YAML::Key << "Light" << YAML::Value << YAML::BeginMap; // Light

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
		if (Collider* collider = entity->GetComponent<Collider>()) {

			if (collider->GetColliderType() == Collider::Type::Sphere) {

				out << YAML::Key << "Sphere Collider" << YAML::Value << YAML::BeginMap; // Sphere Collider

				out << YAML::Key << "Radius" << YAML::Value << ((SphereCollider*) collider)->radius;

				out << YAML::EndMap; // Sphere Collider

			} else if(collider->GetColliderType() == Collider::Type::Box) {

				out << YAML::Key << "Box Collider" << YAML::Value << YAML::BeginMap; // Box Collider

				out << YAML::Key << "Size" << YAML::Value << ((BoxCollider*) collider)->size;

				out << YAML::EndMap; // Box Collider

			}

		}
		if (PhysicsBody* physicsBody = entity->GetComponent<PhysicsBody>()) {

			out << YAML::Key << "Physics" << YAML::Value << YAML::BeginMap; // Physics

			out << YAML::Key << "Mass" << YAML::Value << physicsBody->mass;

			out << YAML::Key << "Gravity" << YAML::Value << physicsBody->gravity;
			out << YAML::Key << "Static" << YAML::Value << physicsBody->staticBody;

			out << YAML::Key << "Linear Velocity" << YAML::Value << physicsBody->linearVelocity;
			out << YAML::Key << "Angular Velocity" << YAML::Value << physicsBody->angularVelocity;


			out << YAML::EndMap; // Physics

		}

		if (ScriptComponent* script = entity->GetComponent<ScriptComponent>()) {

			out << YAML::Key << "Script Component" << YAML::Value << YAML::BeginMap; // Script Component

			out << YAML::Key << "Name" << YAML::Value << script->name;
			out << YAML::Key << "Fields" << YAML::Value << YAML::BeginMap; // Fields
			for (const ScriptField& field : Scripting::GetScriptFields(script->name)) {

				switch (field.type) {

					case ScriptField::Type::Int: SerializeScriptField<int>(field, script, out); break;
					case ScriptField::Type::UInt: SerializeScriptField<unsigned int>(field, script, out); break;
					case ScriptField::Type::Float: SerializeScriptField<float>(field, script, out); break;

					case ScriptField::Type::Vector2: SerializeScriptField<Vector2>(field, script, out); break;
					case ScriptField::Type::Vector3: SerializeScriptField<Vector3>(field, script, out); break;

					case ScriptField::Type::Entity: SerializeScriptField<InternalEntity*>(field, script, out); break;

				}

			}
			out << YAML::EndMap; // Fields

			out << YAML::EndMap; // Script Component

		}
		
		out << YAML::EndMap; // Enity

	}
	void Scene::DeserializeEntity(InternalEntity* entity, const YAML::Node& node) {

		YAML::Node transform = node["Transform"];
		entity->transform->position = transform["Position"].as<Vector3>();
		entity->transform->rotation = transform["Rotation"].as<Vector3>();
		entity->transform->scale = transform["Scale"].as<Vector3>();

		//Set the Parent
		uint32_t parentID = transform["Parent ID"].as<uint32_t>();
		if (parentID == invalidID) entity->transform->parent = nullptr;
		else if(entity->id < parentID) {

			//This little maneuver is gonna cost us 51 miliseconds
			Entity saved = entity;
			InternalEntity* parent = CreateEntityFromID(parentID, "Empty Parent", Vector3::zero, Vector3::zero, Vector3::one);
			entity = saved;

			entity->transform->parent = parent->transform;
			entity->transform->parentChildIndex = (int32_t) parent->transform->children.size();

			parent->transform->children.push_back(entity->id);

		}

		//Set the children
		for (int i = 0; i < transform["Children"].size(); i++) {

			uint32_t childID = transform["Children"][i].as<uint32_t>();
			if (childID < entity->id) continue;
			
			entity->transform->children.push_back(childID);

			Entity saved = entity;
			InternalEntity* child = CreateEntityFromID(childID, "Empty Child", Vector3::zero, Vector3::zero, Vector3::one);
			entity = saved;

			child->transform->parent = entity->transform;
			child->transform->parentChildIndex = i;

		}

		if (YAML::Node rendererNode = node["Mesh Renderer"]) {

			MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();

			YAML::Node meshNode = rendererNode["Mesh"];
			Mesh mesh;

			YAML::Node dataNode = meshNode["Data"];
			for (int i = 0; i < dataNode.size(); i++) {

				YAML::Node vertex = dataNode[i];

				mesh.vertices.push_back(vertex["Position"].as<Vector3>());
				mesh.normals.push_back(vertex["Normal"].as<Vector3>());
				mesh.colors.push_back(vertex["Color"].as<Color>());

			}

			YAML::Node indicesNode = meshNode["Indices"];
			for (int i = 0; i < indicesNode.size(); i++) {

				mesh.indices.push_back(indicesNode[i].as<uint32_t>());

			}

			renderer->meshes.push_back(mesh);

		}
		if (YAML::Node lightNode = node["Light"]) {

			Light* light = entity->AddComponent<Light>();

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

		if (YAML::Node colliderNode = node["Sphere Collider"]) {

			SphereCollider* collider = entity->AddComponent<SphereCollider>();

			collider->radius = colliderNode["Radius"].as<float>();

		}
		if (YAML::Node colliderNode = node["Box Collider"]) {

			BoxCollider* collider = entity->AddComponent<BoxCollider>();

			//collider->size = colliderNode["Size"].as<Vector3>();

		}
		if (YAML::Node physicsBodyNode = node["Physics"]) {

			PhysicsBody* physicsBody = entity->AddComponent<PhysicsBody>();

			physicsBody->mass = physicsBodyNode["Mass"].as<float>();

			physicsBody->staticBody = physicsBodyNode["Static"].as<bool>();
			physicsBody->gravity = physicsBodyNode["Gravity"].as<bool>();

			physicsBody->linearVelocity = physicsBodyNode["Linear Velocity"].as<Vector3>();
			physicsBody->angularVelocity = physicsBodyNode["Angular Velocity"].as<Vector3>();


		}

		if (YAML::Node scriptNode = node["Script Component"]) {

			ScriptComponent* script = entity->AddComponent<ScriptComponent>();
			script->Init(scriptNode["Name"].as<std::string>());

			YAML::Node fields = scriptNode["Fields"];
			for (const ScriptField& field : Scripting::GetScriptFields(script->name)) {

				switch (field.type) {

					case ScriptField::Type::Int: DeserializeScriptField<int>(field, script, fields[field.name]["Value"]); break;
					case ScriptField::Type::UInt: DeserializeScriptField<unsigned int>(field, script, fields[field.name]["Value"]); break;
					case ScriptField::Type::Float: DeserializeScriptField<float>(field, script, fields[field.name]["Value"]); break;

					case ScriptField::Type::Vector2: DeserializeScriptField<Vector2>(field, script, fields[field.name]["Value"]); break;
					case ScriptField::Type::Vector3: DeserializeScriptField<Vector3>(field, script, fields[field.name]["Value"]); break;

					case ScriptField::Type::Entity: DeserializeScriptField<InternalEntity*>(field, script, fields[field.name]["Value"]); break;

				}

			}

		}

	}

	template<typename T> void Scene::SerializeScriptField(const ScriptField& field, ScriptComponent* instance, YAML::Emitter& out) {

		T value;
		instance->GetFieldValue(field, &value);

		out << YAML::Key << field.name << YAML::Value << YAML::BeginMap; // Field

		out << YAML::Key << "Type" << YAML::Value << (int) field.type;
		out << YAML::Key << "Value" << YAML::Value << value;

		out << YAML::EndMap; // Field

	}
	template<typename T> void Scene::DeserializeScriptField(const ScriptField& field, ScriptComponent* instance, const YAML::Node& fieldNode) {

		T tmp = fieldNode.as<T>();
		instance->SetFieldValue(field, &tmp);

	}

}