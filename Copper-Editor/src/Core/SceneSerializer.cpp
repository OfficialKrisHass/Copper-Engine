#include "SceneSerializer.h"

#include "Core/EditorApp.h"

#include "Viewport/SceneCamera.h"

#include <Engine/Components/BoxCollider.h>
#include <Engine/Components/CapsuleCollider.h>
#include <Engine/Components/Light.h>
#include <Engine/Components/MeshRenderer.h>
#include <Engine/Components/RigidBody.h>
#include <Engine/Components/ScriptComponent.h>
#include <Engine/Components/SphereCollider.h>

#include <Engine/Scripting/Script.h>
#include <Engine/Scripting/ScriptingEngine.h>

#include <Engine/YAMLOverloads/Everything.h>

#include <yaml-cpp/yaml.h>

#include <fstream>

using namespace Copper;

namespace Editor::SceneSerializer {

    void SerializeEntity(InternalEntity* entity, YAML::Emitter& out);
    void DeserializeEntity(uint32 id, const YAML::Node& data);

    void SerializeSceneCamera(YAML::Emitter& out);
    void DeserializeSceneCamera(const YAML::Node& data);

    template<typename T> void SerializeField(YAML::Emitter& out, ScriptComponent* instance, const Scripting::Field& field);
    template<typename T> void DeserializeField(const YAML::Node& data, ScriptComponent* instance, const Scripting::Field& field);

    void Serialize(Scene *scene, const fs::path &path) {

        CUP_FUNCTION();

        if (!fs::exists((path.parent_path())))
            fs::create_directories(path.parent_path());

        YAML::Emitter out;
        out << YAML::BeginMap; // Main

        out << YAML::Key << "Version" << YAML::Value << 0;

        // Entities

        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginMap; // Entities
        for (InternalEntity* entity : EntityView(scene))
            SerializeEntity(entity, out);
        out << YAML::EndMap; // Entities;

        SerializeSceneCamera(out);

        out << YAML::EndMap; // Main

        std::ofstream file;
        file.open(path);
        file << out.c_str();
        file.close();

        Log("Scene '{}' serialized.", path.filename().string());

    }
    void Deserialize(Scene* scene, const fs::path &path) {

        CUP_FUNCTION();

        try {

        YAML::Node data = YAML::LoadFile(path.string());

        YAML::Node entities = data["Entities"];
        for (YAML::const_iterator it = entities.begin(); it != entities.end(); ++it)
            DeserializeEntity(it->first.as<uint32>(), it->second);

        if (data["Scene Camera"])
            DeserializeSceneCamera(data["Scene Camera"]);

        } catch (YAML::Exception e) {

            LogError("Could not deserialize scene at path {}. Error: {}", path, e.what());
            return;

        }

#ifdef CU_LOG_STATUS
        if (GetEngineState() == EngineState::PostInitialization)
            LogStatus("\tScene deserialized with {} entities.", scene->GetNumOfEntities());
        else
#endif
            Log("Scene deserialized with {} entities.", scene->GetNumOfEntities());

    }

    void SerializeEntity(InternalEntity* entity, YAML::Emitter& out) {

        CUP_FUNCTION();

        out << YAML::Key << entity->ID() << YAML::Value << YAML::BeginMap; // Entity

        out << YAML::Key << "Name" << YAML::Value << entity->name;

        SerializeEntityTransform(entity->GetTransform(), out);
        SerializeEntityComponents(entity, out);

        out << YAML::EndMap; // Entity

    }
    void DeserializeEntity(uint32 id, const YAML::Node& data) {

        CUP_FUNCTION();

        InternalEntity* entity = CreateEntityFromID(id);
        entity->name = data["Name"].as<std::string>();

        DeserializeEntityTransform(&entity, data["Transform"]);
        DeserializeEntityComponents(entity, data);

    }

    void SerializeEntityTransform(Transform* transform, YAML::Emitter& out) {

        CUP_FUNCTION();

        out << YAML::Key << "Transform" << YAML::Value << YAML::BeginMap; // Transform

        out << YAML::Key << "Position" << YAML::Value << transform->Position();
        out << YAML::Key << "Rotation" << YAML::Value << transform->Rotation();
        out << YAML::Key << "Scale" << YAML::Value << transform->Scale();

        out << YAML::Key << "Parent" << YAML::Value;
        if (transform->Parent()) out << transform->Parent()->GetEntity()->ID();
        else out << INVALID_ENTITY_ID;

        out << YAML::Key << "Children" << YAML::Value << YAML::BeginSeq; // Children
        for (uint32 i = 0; i < transform->NumOfChildren(); i++)
            out << transform->GetChild(i)->GetEntity()->ID();

        out << YAML::EndSeq; // Children
        out << YAML::EndMap; // Transform

    }
    void DeserializeEntityTransform(InternalEntity** entityPtr, const YAML::Node& data) {

        CUP_FUNCTION();

        InternalEntity* entity = *entityPtr;
        Transform* transform = entity->GetTransform();

        transform->SetPosition(data["Position"].as<Vector3>());
        transform->SetRotation(data["Rotation"].as<Quaternion>());
        transform->SetScale(data["Scale"].as<Vector3>());

        uint32 parentID = data["Parent"].as<uint32>();
        if (parentID == INVALID_ENTITY_ID)
            transform->m_parent = nullptr;
        else if (parentID > entity->ID()) {

            uint32 savedID = entity->ID();
            InternalEntity* parent = CreateEntityFromID(parentID);
            entity = *entityPtr = GetEntityFromID(savedID);

            transform->m_parent = parent->GetTransform();
            parent->GetTransform()->m_children.push_back(savedID);

        }

        YAML::Node children = data["Children"];
        for (uint32 i = 0; i < children.size(); i++) {

            uint32 childID = children[i].as<uint32>();
            if (childID < entity->ID()) continue;

            transform->m_children.push_back(childID);

            uint32 savedID = entity->ID();
            InternalEntity* child = CreateEntityFromID(childID);
            entity = GetEntityFromID(savedID);

            child->GetTransform()->m_parent = transform;

        }

    }

    void SerializeEntityComponents(InternalEntity* entity, YAML::Emitter& out) {

        CUP_FUNCTION();

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

            out << YAML::Key << "Mass" << YAML::Value << rb->GetMass();

            out << YAML::Key << "Static" << YAML::Value << rb->GetStatic();
            out << YAML::Key << "Gravity" << YAML::Value << rb->GetGravity();
            out << YAML::Key << "Lock Mask" << YAML::Value << (uint32) rb->GetLockMask();

            out << YAML::EndMap; // Rigid Body

        }

        if (BoxCollider* collider = entity->GetComponent<BoxCollider>()) {

            out << YAML::Key << "Box Collider" << YAML::Value << YAML::BeginMap; // Box Collider

            out << YAML::Key << "Trigger" << YAML::Value << collider->m_trigger;
            out << YAML::Key << "Center" << YAML::Value << collider->m_center;

            out << YAML::Key << "Size" << YAML::Value << collider->m_size;

            out << YAML::EndMap; // Box Collider

        }
        if (SphereCollider* collider = entity->GetComponent<SphereCollider>()) {

            out << YAML::Key << "Sphere Collider" << YAML::Value << YAML::BeginMap; // Sphere Collider

            out << YAML::Key << "Trigger" << YAML::Value << collider->m_trigger;
            out << YAML::Key << "Center" << YAML::Value << collider->m_center;

            out << YAML::Key << "Radius" << YAML::Value << collider->m_radius;

            out << YAML::EndMap; // Sphere Collider

        }
        if (CapsuleCollider* collider = entity->GetComponent<CapsuleCollider>()) {

            out << YAML::Key << "Capsule Collider" << YAML::Value << YAML::BeginMap; // Sphere Collider

            out << YAML::Key << "Trigger" << YAML::Value << collider->m_trigger;
            out << YAML::Key << "Center" << YAML::Value << collider->m_center;

            out << YAML::Key << "Radius" << YAML::Value << collider->m_radius;
            out << YAML::Key << "Height" << YAML::Value << collider->m_height;

            out << YAML::EndMap; // Sphere Collider

        }

        if (ScriptComponent* scriptComponent = entity->GetComponent<ScriptComponent>()) {

            using namespace Scripting;

            const Script* script = GetScript(scriptComponent->GetScriptName());
            if (script == nullptr) {

                LogError("Can't serialize a Script component ({}) on entity {} because it isn't loaded in the Script Map", scriptComponent->GetScriptName(), *entity);
                return;

            }

            out << YAML::Key << "Script Component" << YAML::Value << YAML::BeginMap; // Script Component
            
            out << YAML::Key << "Name" << YAML::Value << scriptComponent->GetScriptName();
            
            out << YAML::Key << "Fields" << YAML::Value << YAML::BeginMap; // Fields

            const std::vector<Field>& fields = script->GetFields();
            for (const Field& field : fields) {

                switch (field.GetType()) {

                case Scripting::Field::Type::None: break;

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
                    out << YAML::Key << "Value" << YAML::Value << (transform ? transform->GetEntity()->ID() : INVALID_ENTITY_ID);

                    out << YAML::EndMap; // Field

                    break;

                }

                default: LogError("Invalid field type {}. Could not serialize on entity {}", (uint8) field.GetType(), *entity); break;

                }

            }

            out << YAML::EndMap; // Fields

            out << YAML::EndMap; // Script Component

        }
        
    }
    void DeserializeEntityComponents(InternalEntity* entity, const YAML::Node& data) {

        CUP_FUNCTION();

        if (YAML::Node rendererNode = data["Mesh Renderer"]) {

            MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();

            renderer->mesh = rendererNode["Mesh"].as<MeshAsset>();
            renderer->material = rendererNode["Material"].as<MaterialAsset>();

        }
        if (YAML::Node lightNode = data["Light"]) {

            Light* light = entity->AddComponent<Light>();

            light->type = (Light::Type) lightNode["Type"].as<uint32>();

            light->color = lightNode["Color"].as<Color>();
            light->intensity = lightNode["Intensity"].as<float>();

        }
        if (YAML::Node camNode = data["Camera"]) {

            Camera* cam = entity->AddComponent<Camera>();

            cam->fov = camNode["Fov"].as<float>();
            cam->nearPlane = camNode["Near Plane"].as<float>();
            cam->farPlane = camNode["Far Plane"].as<float>();

            cam->Resize(camNode["Size"].as<UVector2I>());

        }

        if (YAML::Node rbNode = data["Rigid Body"]) {

            RigidBody* rb = entity->AddComponent<RigidBody>();

            rb->m_mass = rbNode["Mass"].as<float>();

            rb->m_static = rbNode["Static"].as<bool>();
            rb->m_gravity = rbNode["Gravity"].as<bool>();
            rb->m_lockMask = rbNode["Lock Mask"].as<uint8>();

        }

        if (YAML::Node colliderNode = data["Box Collider"]) {

            BoxCollider* collider = entity->AddComponent<BoxCollider>();

            collider->m_trigger = colliderNode["Trigger"].as<bool>();
            collider->m_center = colliderNode["Center"].as<Vector3>();

            collider->m_size = colliderNode["Size"].as<Vector3>();

        }
        if (YAML::Node colliderNode = data["Sphere Collider"]) {

            SphereCollider* collider = entity->AddComponent<SphereCollider>();

            collider->m_trigger = colliderNode["Trigger"].as<bool>();
            collider->m_center = colliderNode["Center"].as<Vector3>();

            collider->m_radius = colliderNode["Radius"].as<float>();

        }
        if (YAML::Node colliderNode = data["Capsule Collider"]) {

            CapsuleCollider* collider = entity->AddComponent<CapsuleCollider>();

            collider->m_trigger = colliderNode["Trigger"].as<bool>();
            collider->m_center = colliderNode["Center"].as<Vector3>();

            collider->m_radius = colliderNode["Radius"].as<float>();
            collider->m_height = colliderNode["Height"].as<float>();

        }

        if (YAML::Node scriptNode = data["Script Component"]) {

            std::string name = scriptNode["Name"].as<std::string>();
            const Scripting::Script* script = Scripting::GetScript(name);

            if (script == nullptr) {

                LogError("Could not deserialize Script Component '{}' on Entity '{}'. Does not exist in loaded Script Map", name, *entity);
                return;

            }

            ScriptComponent* scriptComponent = entity->AddComponent<ScriptComponent>();
            scriptComponent->Setup(script);

            // Fields

            YAML::Node fieldsNode = scriptNode["Fields"];
            const std::vector<Scripting::Field>& fields = script->GetFields();

            for (const Scripting::Field& field : fields) {

                YAML::Node fieldNode = fieldsNode[field.GetName()];

                if (!fieldNode) {

                    LogError("Field '{}' has not been serialized", field.GetName());
                    continue;

                }
                if (fieldNode["Type"].as<uint32>() != (uint32) field.GetType()) continue;

                switch (field.GetType()) {

                case Scripting::Field::Type::None: break;

                case Scripting::Field::Type::Int: DeserializeField<int32>(fieldNode, scriptComponent, field); break;
                case Scripting::Field::Type::UInt: DeserializeField<uint32>(fieldNode, scriptComponent, field); break;
                case Scripting::Field::Type::Float: DeserializeField<float>(fieldNode, scriptComponent, field); break;
                case Scripting::Field::Type::Double: DeserializeField<double>(fieldNode, scriptComponent, field); break;

                case Scripting::Field::Type::Vector2: DeserializeField<Vector2>(fieldNode, scriptComponent, field); break;
                case Scripting::Field::Type::Vector3: DeserializeField<Vector3>(fieldNode, scriptComponent, field); break;
                case Scripting::Field::Type::Quaternion: DeserializeField<Quaternion>(fieldNode, scriptComponent, field); break;

                case Scripting::Field::Type::Entity: {

                    CUP_SCOPE("Entity Field deserialization", 457);

                    uint64 id = fieldNode["Value"].as<uint64>();
                    if (id == INVALID_ENTITY_ID) break;

                    if (id > entity->ID()) {

                        uint32 tmp = entity->ID();
                        CreateEntityFromID((uint32) id);
                        entity = GetEntityFromID(tmp);

                    }

                    field.SetRefValue(scriptComponent, (void*) id);
                    break;

                }
                case Scripting::Field::Type::Transform: {

                    CUP_SCOPE("Transform Field deserialization", 457);

                    uint32 id = fieldNode["Value"].as<uint32>();
                    if (id == INVALID_ENTITY_ID) break;

                    if (id > entity->ID()) {

                        uint32 tmp = entity->ID();
                        CreateEntityFromID(id);
                        entity = GetEntityFromID(tmp);

                    }

                    Transform* transform = GetEntityFromID(id)->GetTransform();
                    field.SetRefValue(scriptComponent, transform);
                    break;

                }

                }

            }

        }

    }

    void SerializeSceneCamera(YAML::Emitter& out) {

        CUP_FUNCTION();

        out << YAML::Key << "Scene Camera" << YAML::Value << YAML::BeginMap;

        const SceneCamera& sceneCam = GetSceneCam();

        out << YAML::Key << "Position" << YAML::Value << sceneCam.GetTransform()->Position();
        out << YAML::Key << "Rotation" << YAML::Value << sceneCam.GetTransform()->Rotation();

        out << YAML::Key << "Speed" << YAML::Value << sceneCam.speed;
        out << YAML::Key << "Sensitivity" << YAML::Value << sceneCam.sensitivity;

        out << YAML::EndMap;

    }
    void DeserializeSceneCamera(const YAML::Node& data) {

        CUP_FUNCTION();

        SceneCamera& sceneCam = GetSceneCam();

        sceneCam.GetTransform()->SetPosition(data["Position"].as<Vector3>());
        sceneCam.GetTransform()->SetRotation(data["Rotation"].as<Quaternion>());

        sceneCam.speed = data["Speed"].as<float>();
        sceneCam.sensitivity = data["Sensitivity"].as<float>();

        sceneCam.UpdateRotation();

    }

    template<typename T> void SerializeField(YAML::Emitter& out, ScriptComponent* instance, const Scripting::Field& field) {

        CUP_FUNCTION();

        T value;
        field.GetValue(instance, &value);

        out << YAML::Key << field.GetName() << YAML::Value << YAML::BeginMap; // Field

        out << YAML::Key << "Type" << YAML::Value << (uint32) field.GetType();
        out << YAML::Key << "Value" << value;

        out << YAML::EndMap; // Field

    }
    template<typename T> void DeserializeField(const YAML::Node& fieldNode, ScriptComponent* instance, const Scripting::Field& field) {

        CUP_FUNCTION();

        T tmp = fieldNode["Value"].as<T>();
        field.SetValue(instance, &tmp);

    }

}
