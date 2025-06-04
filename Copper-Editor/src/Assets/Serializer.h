#pragma once

#include <Copper.h>

#include <Engine/AssetStorage/AssetPtr.h>

namespace Copper { class Material; }

namespace Editor::AssetFile {

    using namespace Copper;

    void SerializeMaterial(const Copper::fs::path& path, const AssetPtr<Material>& material);
    AssetPtr<Material> DeserializeMaterial(const Copper::fs::path& path, const Copper::UUID& uuid);

}
