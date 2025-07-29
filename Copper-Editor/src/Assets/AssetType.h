#pragma once

#include <Copper.h>

namespace Editor {

    using namespace Copper;

    enum class AssetType : uint8 {

        None = 0,

        Scene, // .copper

        Script, // .cs

        // Marker for PAD tracked assets. All Values after this are larger numerically.
        // To check if an AssetType is tracked by the PAD simply check if it's larger than DATABASE_ASSETS
        DATABASE_ASSETS = 100,

        Texture, // .png, .jpg
        Material, // .mat
        Model, // .fbx
        
    };

    AssetType GetAssetTypeFromExtension(std::string_view extension);

    inline bool IsDatabaseAsset(AssetType type) {

        return type > AssetType::DATABASE_ASSETS;

    }

}
