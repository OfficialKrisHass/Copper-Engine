#pragma once

#include "Engine/Core/Core.h"

#include "Engine/AssetStorage/AssetMap.h"
#include "Engine/AssetStorage/AssetStorage.h"

namespace Copper {

    template<typename AssetType> class AssetPtr {

    public:
        AssetPtr() = default;
        AssetPtr(const UUID& uuid) : m_key(uuid) {}
        AssetPtr(const AssetPtr& other) : m_key(other.m_key) {}

        // Main functionality

        inline AssetType* operator->() const { return AssetStorage::GetAssetMap<AssetType>().GetRaw(m_key); }

        // Getters

        inline const UUID& AssetUUID() const { return m_key; }
        inline bool IsValid() const { return AssetStorage::GetAssetMap<AssetType>().GetRaw(m_key) != nullptr; }

        // Assignement operators

        inline AssetPtr& operator=(const AssetPtr& other) {

            m_key = other.m_key;
            return *this;

        }
        inline AssetPtr& operator=(const UUID& other) {

            m_key = other;
            return *this;

        }

        // Comparison operators

        inline bool operator==(const UUID& other) const { return m_key == other; }
        inline bool operator==(const AssetPtr& other) const { return m_key == other.m_key; }
        inline bool operator!=(const UUID& other) const { return !(*this == other); }
        inline bool operator!=(const AssetPtr& other) const { return !(*this == other); }

        // Cast operators

        inline operator AssetType*() const { return AssetStorage::GetAssetMap<AssetType>().GetRaw(m_key); }

    private:
        UUID m_key = UUID();

    };

    typedef AssetPtr<class Material> MaterialAsset;
    typedef AssetPtr<class Texture> TextureAsset;
    typedef AssetPtr<class Mesh> MeshAsset;

}
