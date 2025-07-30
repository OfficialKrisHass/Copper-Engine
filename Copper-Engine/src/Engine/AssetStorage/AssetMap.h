#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/UUID.h"

#include <unordered_map>

namespace Copper {

    template<typename AssetType> class AssetPtr;

    // An UUID-AssetType Hash map
    template<typename AssetType> class AssetMap {

        friend AssetPtr<AssetType>;

    public:
        typedef std::unordered_map<UUID, AssetType> UnderlyingMap;

        template<typename... Args> inline AssetPtr<AssetType> Create(Args&&... args) {

            CUP_FUNCTION();

            UUID uuid = UUID::Generate();
            CU_ASSERT(!m_map.contains(uuid), "Attempted to create a new asset but the RANDOMLY generated uuid ({}) is already present in the {} asset map. WHAT THE FUCK ???????????", uuid.ToString(), typeid(AssetType).name());

            return Insert(uuid, std::forward<Args>(args)...);

        }
        template<typename... Args> inline AssetPtr<AssetType> Insert(const UUID& uuid, Args&&... args) {

            CUP_FUNCTION();

            m_map.try_emplace(uuid, std::forward<Args>(args)...);
            return AssetPtr<AssetType>(uuid);

        }

        inline AssetPtr<AssetType> Get(const UUID& uuid) {

            CUP_FUNCTION();

            if (!m_map.contains(uuid)) return AssetPtr<AssetType>();
            return AssetPtr<AssetType>(uuid);

        }

        inline void Delete(const UUID& uuid) {

            CUP_FUNCTION();

            auto it = m_map.find(uuid);
            CU_EDITOR_ASSERT(it != m_map.end(), "Can't delete an asset that doesn't exist in the map. UUID: {}", uuid.ToString());

            m_map.erase(it);

        }

        inline bool Contains(const UUID& uuid) {

            CUP_FUNCTION();

            return m_map.contains(uuid);

        }

    private:
        UnderlyingMap m_map;

        inline AssetType* GetRaw(const UUID& uuid) {

            CUP_FUNCTION();

            auto it = m_map.find(uuid);
            if (it == m_map.end()) return nullptr;

            return &it->second;

        }

    };

}
