#include "Clipboard.h"

#include "Core/SceneSerializer.h"

#include <Engine/Scene/InternalEntity.h>

#include <yaml-cpp/yaml.h>

namespace Editor {

    std::string data;

    void CopyToClipboard(InternalEntity* entity) {

        CUP_FUNCTION();

        YAML::Emitter out;

        out << YAML::BeginMap;

        out << YAML::Key << "Name" << YAML::Value << entity->name;

        Transform* parent = entity->GetTransform()->Parent();
        std::vector<uint32> children = entity->GetTransform()->m_children;

        entity->GetTransform()->m_parent = nullptr;
        entity->GetTransform()->m_children.clear();

        SceneSerializer::SerializeEntityTransform(entity->GetTransform(), out);
        SceneSerializer::SerializeEntityComponents(entity, out);

        entity->GetTransform()->m_parent = parent;
        entity->GetTransform()->m_children = children;

        out << YAML::EndMap;

        data = out.c_str();

    }
    InternalEntity* PasteFromClipboard() {

        CUP_FUNCTION();

        YAML::Node node = YAML::Load(data);

        std::string name = node["Name"].as<std::string>() + " Copy";
        InternalEntity* entity = CreateEntity(Vector3::zero, Quaternion::identity, Vector3::one, name);

        SceneSerializer::DeserializeEntityTransform(&entity, node["Transform"]);
        SceneSerializer::DeserializeEntityComponents(entity, node);

        return entity;

    }

    bool ClipboardEmpty() { return data.empty(); }

}
