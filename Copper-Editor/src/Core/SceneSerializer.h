#pragma once

#include <Copper.h>

namespace Editor::SceneSerializer {

    void Serialize(Copper::Scene* scene, const Copper::fs::path& path);
    void Deserialize(Copper::Scene* scene, const Copper::fs::path& path);

}
