#include "cupch.h"
#include "Scene.h"

#include "Engine/Events/RegistryEvent.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Light.h"

#include "Engine/Components/RigidBody.h"

#include "Engine/Components/Collider.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/SphereCollider.h"
#include "Engine/Components/CapsuleCollider.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/Script.h"
#include "Engine/Scripting/Field.h"
#include "Engine/Scripting/ManagedReferences.h"
#include "Engine/Scripting/Classes.h"

#include "Engine/Physics/Raycast.h"

#include "Engine/Input/Popup.h"

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

        for (InternalEntity* entity : EntityView(this)) {

            // Initialize physics

            if (RigidBody* rb = entity->GetComponent<RigidBody>())
                rb->Setup();
            else if (Collider* collider = entity->GetComponent<Collider>())
                collider->Setup();

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
                script->OnBegin();

            script->OnUpdate();

        }

	}

	void Scene::Render(Camera* cam, bool gizmos) {

        CUP_FUNCTION();
        Renderer::Render(cam, gizmos);

    }

    bool Scene::EntityCreated(const Event& e) {

        CUP_FUNCTION();

        EntityEvent* event = (EntityEvent*) &e;

        Scripting::CreateManagedReference((void*) (uint64) event->entity->m_id, Scripting::EntityClass());
        Scripting::CreateManagedReference(event->entity->m_transform, Scripting::TransformClass());

        return true;

    }
    bool Scene::EntityRemoved(const Event& e) {

        CUP_FUNCTION();

        EntityEvent* event = (EntityEvent*) &e;

        Scripting::RemoveManagedReference((void*) (uint64) event->entity->m_id);
        Scripting::RemoveManagedReference(event->entity->m_transform);

        return true;

    }

    bool Scene::ComponentAdded(const Event& e) {

        CUP_FUNCTION();

        ComponentEvent* event = (ComponentEvent*) &e;

        if (event->componentID == CAMERA_CID)
            Scripting::CreateManagedReference(event->component, Scripting::CameraClass());
        else if (event->componentID == LIGHT_CID)
            Scripting::CreateManagedReference(event->component, Scripting::LightClass());

        return true;

    }
    bool Scene::ComponentRemoved(const Event& e) {

        CUP_FUNCTION();

        ComponentEvent* event = (ComponentEvent*) &e;
        Scripting::RemoveManagedReference(event->component);

        return true;

    }

	void Scene::Serialize(const fs::path& path) {

		// TODO: Change the format of serializing the Scene
		// Why am I even writing these TODOs, there are 22 other TODOs that I have never even
		// looked at after writing them, this one will  probably stay here for eternity

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

		AddEntityCreatedEventFunc(BindEventFunc(Scene::EntityCreated));
		AddEntityRemovedEventFunc(BindEventFunc(Scene::EntityRemoved));

		AddComponentAddedEventFunc(BindEventFunc(Scene::ComponentAdded));
		AddComponentRemovedEventFunc(BindEventFunc(Scene::ComponentRemoved));

		m_runtimeRunning = false;
		m_runtimeStarted = false;
		m_physicsInitialized = false;

		cam = nullptr;

		try {

		YAML::Node data = YAML::LoadFile(path.string());

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

		} catch (YAML::Exception e) {

			Input::ErrorPopup("Failed to open Scene", "Could not open the scene file.\n\nPath:\n" + path.string() + "\n\nError Message:\n" + e.what());
			return false;
		}
		
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

        if (ScriptComponent* scriptComponent = entity->GetComponent<ScriptComponent>()) {

            using namespace Scripting;

            out << YAML::Key << "Script Component" << YAML::Value << YAML::BeginMap; // Script Component
            
            const Script* script = scriptComponent->GetScript();
            out << YAML::Key << "Name" << YAML::Value << script->FullName();
            
            out << YAML::Key << "Fields" << YAML::Value << YAML::BeginMap; // Fields

            const std::vector<Field>& fields = script->GetFields();
            for (const Field& field : fields) {

                switch (field.GetType()) {

                case Scripting::Field::Type::Int: SerializeField<int32>(out, scriptComponent, field); break;
                case Scripting::Field::Type::UInt: SerializeField<uint32>(out, scriptComponent, field); break;
                case Scripting::Field::Type::Float: SerializeField<float>(out, scriptComponent, field); break;
                case Scripting::Field::Type::Double: SerializeField<double>(out, scriptComponent, field); break;
                                                     
                case Scripting::Field::Type::Vector2: SerializeField<Vector2>(out, scriptComponent, field); break;
                case Scripting::Field::Type::Vector3: SerializeField<Vector3>(out, scriptComponent, field); break;
                case Scripting::Field::Type::Quaternion: SerializeField<Quaternion>(out, scriptComponent, field); break;

                case Scripting::Field::Type::Entity: {

                    uint64 id;
                    field.GetRefValue(scriptComponent, (void**) &id, (void*) INVALID_ENTITY_ID);

                    out << YAML::Key << field.GetName() << YAML::Value << YAML::BeginMap; // Field

                    out << YAML::Key << "Type" << YAML::Value << (uint32) field.GetType();
                    out << YAML::Key << "Value" << YAML::Value << (uint32) id;

                    out << YAML::EndMap; // Field
                    
                    break;


                }
                case Scripting::Field::Type::Transform: {
                    
                    Transform* transform = nullptr;
                    field.GetRefValue(scriptComponent, (void**) &transform);

                    out << YAML::Key << field.GetName() << YAML::Value << YAML::BeginMap; // Field

                    out << YAML::Key << "Type" << YAML::Value << (uint32) field.GetType();
                    out << YAML::Key << "Value" << YAML::Value << (transform ? transform->m_entity->m_id : INVALID_ENTITY_ID);

                    out << YAML::EndMap; // Field

                    break;

                }

                }

            }

            out << YAML::EndMap; // Fields

            out << YAML::EndMap; // Script Component

        }

		out << YAML::EndMap; // Enity

	}
	void Scene::DeserializeEntity(InternalEntity* entity, const YAML::Node& node) {

		CUP_FUNCTION();

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

			ScriptComponent* scriptComponent = entity->AddComponent<ScriptComponent>();

            std::string name = scriptNode["Name"].as<std::string>();
            const Scripting::ScriptMap& scriptMap = Scripting::ComponentScripts();

            if (scriptMap.find(name) == scriptMap.end()) {

                LogError("Could not deserialize Script Component '{}' on Entity '{}'. Does not exist in loaded Script Map", name, *entity);
                return;

            }

            const Scripting::Script& script = scriptMap.at(name);
            scriptComponent->Setup(&script);

            // Fields

            YAML::Node fieldsNode = scriptNode["Fields"];
            const std::vector<Scripting::Field>& fields = script.GetFields();

            for (const Scripting::Field& field : fields) {

                YAML::Node fieldNode = fieldsNode[field.GetName()];

                if (!fieldNode) {

                    LogError("Field '{}' has not been serialized", field.GetName());
                    continue;

                }
                if (fieldNode["Type"].as<uint32>() != (uint32) field.GetType()) continue;
                
                switch (field.GetType()) {

                case Scripting::Field::Type::Int: DeserializeField<int32>(fieldNode, scriptComponent, field); break;
                case Scripting::Field::Type::UInt: DeserializeField<uint32>(fieldNode, scriptComponent, field); break;
                case Scripting::Field::Type::Float: DeserializeField<float>(fieldNode, scriptComponent, field); break;
                case Scripting::Field::Type::Double: DeserializeField<double>(fieldNode, scriptComponent, field); break;

                case Scripting::Field::Type::Vector2: DeserializeField<Vector2>(fieldNode, scriptComponent, field); break;
                case Scripting::Field::Type::Vector3: DeserializeField<Vector3>(fieldNode, scriptComponent, field); break;
                case Scripting::Field::Type::Quaternion: DeserializeField<Quaternion>(fieldNode, scriptComponent, field); break;

                case Scripting::Field::Type::Entity: {

                    uint64 id = fieldNode["Value"].as<uint32>();
                    if (id == INVALID_ENTITY_ID) break;

					if (id > entity->m_id) {

						uint32 tmp = entity->m_id;
						CreateEntityFromID(id);
						entity = m_registry.GetEntityFromID(tmp);

					}

                    field.SetRefValue(scriptComponent, (void*) id);
                    break;

                }
                case Scripting::Field::Type::Transform: {

                    uint32 id = fieldNode["Value"].as<uint32>();
                    if (id == INVALID_ENTITY_ID) break;

                    if (id > entity->m_id) {

						uint32 tmp = entity->m_id;
						CreateEntityFromID(id);
						entity = m_registry.GetEntityFromID(tmp);

					}

                    Transform* transform = GetEntityFromID(id)->m_transform;
                    field.SetRefValue(scriptComponent, transform);
                    break;

                }

                }


            }

		}

	}

    template<typename T> void Scene::SerializeField(YAML::Emitter& out, ScriptComponent* instance, const Scripting::Field& field) {

        CUP_FUNCTION();

        T value;
        field.GetValue(instance, &value);

        out << YAML::Key << field.GetName() << YAML::Value << YAML::BeginMap; // Field

        out << YAML::Key << "Type" << YAML::Value << (uint32) field.GetType();
        out << YAML::Key << "Value" << value;

        out << YAML::EndMap; // Field

    }
    template<typename T> void Scene::DeserializeField(const YAML::Node& fieldNode, ScriptComponent* instance, const Scripting::Field& field) {

        CUP_FUNCTION();

        T tmp = fieldNode["Value"].as<T>();
        field.SetValue(instance, &tmp);

    }

}
