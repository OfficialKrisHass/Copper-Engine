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

            UUID uuid;
            CU_ASSERT(!m_map.contains(uuid), "Attempted to create a new asset but the RANDOMLY generated uuis is already present in the {} asset map. WHAT THE FUCK ???????????", typeid(AssetType).name());

            return Insert(uuid, std::forward<Args>(args)...);

        }
        template<typename... Args> inline AssetPtr<AssetType> Insert(const UUID& uuid, Args&&... args) {

            CUP_FUNCTION();

            m_map.emplace(std::piecewise_construct, std::forward_as_tuple(uuid), std::forward_as_tuple(std::forward<Args>(args)...));
            return AssetPtr<AssetType>(uuid);

        }

        AssetPtr<AssetType> inline Get(const UUID& uuid) {

            CUP_FUNCTION();

            if (!m_map.contains(uuid)) return AssetPtr<AssetType>();
            return AssetPtr<AssetType>(uuid);

        }

        inline void Delete(const UUID& uuid) {

            CUP_FUNCTION();

            auto it = m_map.find(uuid);
            if (it == m_map.end()) {

                LogError("Can't remove an Asset that doesn't exist in the map:\n\tUUID: {}", uuid.ToString());
                return;

            }

            m_map.erase(it);

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
