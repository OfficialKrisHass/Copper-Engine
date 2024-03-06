#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/UUID.h"

#include <unordered_map>

namespace Copper {

	template<typename AssetType> class AssetPtr;

	// A hash map of UUID-AssetType key-value type
	template<typename AssetType> class AssetMap {

	public:
		template<typename... Args> AssetPtr<AssetType> Create(Args&&... args) {

			CUP_FUNCTION();

			UUID uuid = GetUUID();
			
			m_map[uuid] = AssetType(args...);
			return AssetPtr<AssetType>(uuid);

		}
		template<typename... Args> AssetType* CreateRaw(Args&&... args) {

			CUP_FUNCTION();

			UUID uuid = GetUUID();

			m_map[uuid] = AssetType(args...);
			return &m_map[uuid];

		}
		AssetPtr<AssetType> Get(const UUID& uuid) {

			CUP_FUNCTION();

			if (m_map.find(uuid) == m_map.end()) return AssetPtr<AssetType>();
			return AssetPtr<AssetType>(uuid);

		}
		AssetType* GetRaw(const UUID& uuid) {

			CUP_FUNCTION();

			if (m_map.find(uuid) == m_map.end()) return nullptr;
			return &m_map.at(uuid);

		}
		void Delete(const UUID& uuid) {

			CUP_FUNCTION();

			if (m_map.find(uuid) == m_map.end()) {

				LogError("Can't remove an Asset that doesn't exist in the map:\n\tUUID: {}", uuid.bytes());
				return;

			}

			m_map.erase(uuid);

		}

	private:
		std::unordered_map<UUID, AssetType> m_map;

	};

}