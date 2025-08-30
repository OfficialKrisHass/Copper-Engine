#include "FieldSerializer.h"

#include <Engine/Components/ScriptComponent.h>

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace Editor {

    template<typename T> void SerializeField(YAML::Emitter& out, ScriptComponent* instance, const Scripting::Field& field);
    template<typename T> void DeserializeField(const YAML::Node& data, ScriptComponent* instance, const Scripting::Field& field);

    void SaveFields(const fs::path &path) {

        CUP_FUNCTION();

        YAML::Emitter out;
        out << YAML::BeginMap; // Main

        for (ScriptComponent* component : ComponentView<ScriptComponent>(GetScene())) {

            CU_ASSERT(component != nullptr, "Invalid component received from ComponentView<ScriptComponent>");

            out << YAML::Key << component->GetEntity()->GetID() << YAML::Value << YAML::BeginMap; // Component

            const std::vector<Scripting::Field>& fields = component->GetScript()->GetFields();
            for (const Scripting::Field& field : fields) {

                switch (field.GetType()) {

                case Scripting::Field::Type::None: break;

                case Scripting::Field::Type::Int: SerializeField<int32>(out, component, field); break;
                case Scripting::Field::Type::UInt: SerializeField<uint32>(out, component, field); break;
                case Scripting::Field::Type::Float: SerializeField<float>(out, component, field); break;
                case Scripting::Field::Type::Double: SerializeField<double>(out, component, field); break;

                case Scripting::Field::Type::Vector2: SerializeField<Vector2>(out, component, field); break;
                case Scripting::Field::Type::Vector3: SerializeField<Vector3>(out, component, field); break;
                case Scripting::Field::Type::Quaternion: SerializeField<Quaternion>(out, component, field); break;

                case Scripting::Field::Type::Entity: {

                    uint64 id;
                    field.GetRefValue(component, (void**) &id, (void*) INVALID_ENTITY_ID);

                    if (GetEntityFromID(id) == nullptr && id != INVALID_ENTITY_ID) {

                        LogError("Entity field '{}' on Entity '{}' has an invalid, presumably missing value. Value (entity ID): {}", field.GetName(), *component->GetEntity(), id);

                        field.SetValue(component, nullptr);
                        id = INVALID_ENTITY_ID;

                    }

                    out << YAML::Key << field.GetName() << YAML::Value << YAML::BeginMap; // Field

                    out << YAML::Key << "Type" << YAML::Value << (uint32) field.GetType();
                    out << YAML::Key << "Value" << YAML::Value << (uint32) id;

                    out << YAML::EndMap; // Field

                    break;


                }
                case Scripting::Field::Type::Transform: {

                    Transform* transform = nullptr;
                    field.GetRefValue(component, (void**) &transform);

                    if (transform != nullptr && !transform->GetEntity().IsValid() && transform->GetEntity().ID() != INVALID_ENTITY_ID) {

                        LogError("Transform field '{}' on Entity '{}' has an invalid, presumably missing value. Value (entity ID): {}", field.GetName(), *component->GetEntity(), transform->GetEntity().ID());

                        field.SetValue(component, nullptr);
                        transform = nullptr;

                    }

                    out << YAML::Key << field.GetName() << YAML::Value << YAML::BeginMap; // Field

                    out << YAML::Key << "Type" << YAML::Value << (uint32) field.GetType();
                    out << YAML::Key << "Value" << YAML::Value << (transform != nullptr ? transform->GetEntity()->GetID() : INVALID_ENTITY_ID);

                    out << YAML::EndMap; // Field

                    break;

                }

                default: LogError("Invalid field type {}. Could not serialize on entity {}", (uint8) field.GetType(), *component->GetEntity()); break;

                }

            }

            out << YAML::EndMap; // Component

        }

        out << YAML::EndMap; // Main

        std::ofstream file = std::ofstream(path);
        file << out.c_str();

    }
    void LoadFields(const fs::path &path) {

        CUP_FUNCTION();

        if (!fs::exists(path))
            return LogError("Can't load fields from a nonexistent file. Path: '{}'", path);

        YAML::Node data = YAML::LoadFile(path.string());
        for (YAML::const_iterator it = data.begin(); it != data.end(); ++it) {

            InternalEntity* entity = GetEntityFromID(it->first.as<uint32>());

            CU_ASSERT(entity->HasComponent<ScriptComponent>(), "Entity '{}' does not have a script component.", *entity);
            ScriptComponent* component = entity->GetComponent<ScriptComponent>();

            YAML::Node fieldsNode = it->second;
            const std::vector<Scripting::Field>& fields = component->GetScript()->GetFields();

            for (const Scripting::Field& field : fields) {

                YAML::Node fieldNode = fieldsNode[field.GetName()];

                if (!fieldNode) continue;
                if (fieldNode["Type"].as<uint32>() != static_cast<uint32>(field.GetType())) continue;

                switch (field.GetType()) {

                case Scripting::Field::Type::None: break;

                case Scripting::Field::Type::Int: DeserializeField<int32>(fieldNode, component, field); break;
                case Scripting::Field::Type::UInt: DeserializeField<uint32>(fieldNode, component, field); break;
                case Scripting::Field::Type::Float: DeserializeField<float>(fieldNode, component, field); break;
                case Scripting::Field::Type::Double: DeserializeField<double>(fieldNode, component, field); break;

                case Scripting::Field::Type::Vector2: DeserializeField<Vector2>(fieldNode, component, field); break;
                case Scripting::Field::Type::Vector3: DeserializeField<Vector3>(fieldNode, component, field); break;
                case Scripting::Field::Type::Quaternion: DeserializeField<Quaternion>(fieldNode, component, field); break;

                case Scripting::Field::Type::Entity: {

                    CUP_SCOPE("Entity Field deserialization", 457);

                    uint32 id = fieldNode["Value"].as<uint32>();

                    if (GetEntityFromID(id) == nullptr) {

                        if (id != INVALID_ENTITY_ID)
                            LogError("Entity field '{}' on Entity '{}' has an invalid, presumably missing value. Value (entity ID): {}", field.GetName(), *component->GetEntity(), id);

                        field.SetValue(component, nullptr);
                        break;

                    }

                    field.SetRefValue(component, (void*) static_cast<uint64>(id));
                    break;

                }
                case Scripting::Field::Type::Transform: {

                    CUP_SCOPE("Transform Field deserialization", 457);

                    uint32 id = fieldNode["Value"].as<uint32>();

                    if (GetEntityFromID(id) == nullptr) {

                        if (id != INVALID_ENTITY_ID)
                            LogError("Transform field '{}' on Entity '{}' has an invalid, presumably missing value. Value (entity ID): {}", field.GetName(), *component->GetEntity(), id);

                        field.SetValue(component, nullptr);
                        break;

                    }

                    Transform* transform = GetEntityFromID(id)->GetTransform();
                    field.SetRefValue(component, transform);

                    break;

                }

                }

            }

        }

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
