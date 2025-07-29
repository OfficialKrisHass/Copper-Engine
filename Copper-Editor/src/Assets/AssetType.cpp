#include "AssetType.h"

namespace Editor {

    static const std::unordered_map<std::string_view, AssetType> extensionMap = {

        { ".copper", AssetType::Scene },

        { ".cs", AssetType::Script },

        { ".png", AssetType::Texture },
        { ".jpg", AssetType::Texture },

        { ".mat", AssetType::Material },

        { ".fbx", AssetType::Model },

    };

    AssetType GetAssetTypeFromExtension(std::string_view extension) {

        CUP_FUNCTION();

        const auto it = extensionMap.find(extension);
        if (it == extensionMap.end()) return AssetType::None;

        return it->second;

    }

}
