#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/UUID.h"

#include <unordered_map>

namespace Copper {

	template<typename AssetType> class AssetPtr;

	// An UUID-AssetType Hash map
	template<typename AssetType> class AssetMap {

	public:
		template<typename... Args> inline AssetPtr<AssetType> Create(Args&&... args) {

			CUP_FUNCTION();

			UUID uuid = GetUUID();

			m_map[uuid] = AssetType(args...);
			return AssetPtr<AssetType>(uuid);

		}
		template<typename... Args> inline AssetPtr<AssetType> Insert(const UUID& uuid, Args&&... args) {

			CUP_FUNCTION();

			m_map[uuid] = AssetType(args...);
			return AssetPtr<AssetType>(uuid);

		}

		AssetPtr<AssetType> inline Get(const UUID& uuid) {

			CUP_FUNCTION();

			if (!m_map.contains(uuid)) return AssetPtr<AssetType>();
			return AssetPtr<AssetType>(uuid);

		}

		inline void Delete(const UUID& uuid) {

			CUP_FUNCTION();

			if (!m_map.contains(uuid)) {

				LogError("Can't remove an Asset that doesn't exist in the map:\n\tUUID: {}", uuid.bytes());
				return;

			}

			m_map.erase(uuid);

		}

		// Raw versions

		template<typename... Args> inline AssetType* CreateRaw(Args&&... args) {

			CUP_FUNCTION();

			UUID uuid = GetUUID();

			m_map[uuid] = AssetType(args...);
			return &m_map[uuid];

		}
		template<typename... Args> inline AssetType* InsertRaw(const UUID& uuid, Args&&... args) {

			CUP_FUNCTION();

			m_map[uuid] = AssetType(args...);
			return &m_map[uuid];

		}

		inline AssetType* GetRaw(const UUID& uuid) {

			CUP_FUNCTION();

			if (!m_map.contains(uuid)) return nullptr;
			return &m_map.at(uuid);

		}

	private:
		std::unordered_map<UUID, AssetType> m_map;

	};

}