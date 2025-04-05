#pragma once

#include <Copper.h>

namespace Editor::SceneSerializer {

    using namespace Copper;

    void Serialize(Scene* scene, const fs::path& path);
    void Deserialize(Scene* scene, const fs::path& path);

    void SerializeEntityTransform(Transform* transform, YAML::Emitter& out);
    void DeserializeEntityTransform(InternalEntity** entityPtr, const YAML::Node& data);

    void SerializeEntityComponents(InternalEntity* entity, YAML::Emitter& out);
    void DeserializeEntityComponents(InternalEntity* entity, const YAML::Node& data);

}
