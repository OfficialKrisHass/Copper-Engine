#pragma once

#include <Copper.h>

namespace Editor::ProjectMetadata {

    void Serialize(const std::unordered_map<Copper::fs::path, Copper::UUID>& assetMetas);
    void Deserialize(std::unordered_map<Copper::fs::path, Copper::UUID>& assetMetas);

}
