#pragma once

#include "Engine/Core/Core.h"

#include "Engine/AssetStorage/AssetMap.h"

namespace Copper::AssetStorage {

	template<typename AssetType> AssetMap<AssetType>& GetAssetMap();

	template<typename AssetType, typename... Args> inline AssetPtr<AssetType> CreateAsset(Args&&... args) {
		
		CUP_FUNCTION();

		return GetAssetMap<AssetType>().Create(args...);

	}
	template<typename AssetType, typename... Args> inline AssetPtr<AssetType> InsertAsset(const UUID& uuid, Args&&... args) {

		CUP_FUNCTION();

		return GetAssetMap<AssetType>().Insert(uuid, args...);

	}

	template<typename AssetType> inline AssetPtr<AssetType> GetAsset(const UUID& uuid) {
		
		CUP_FUNCTION();

		return GetAssetMap<AssetType>().Get(uuid);
	
	}

	template<typename AssetType> inline void DeleteAsset(const UUID& uuid) {
		
		CUP_FUNCTION();

		GetAssetMap<AssetType>().Delete(uuid);
	
	}
	template<typename AssetType> inline  void DeleteAsset(const AssetPtr<AssetType>& asset) {

		CUP_FUNCTION();

		GetAssetMap<AssetType>().Delete(asset);

	}

	// Raw versions

	template<typename AssetType, typename... Args> inline AssetType* CreateAssetRaw(Args&&... args) {

		CUP_FUNCTION();

		return GetAssetMap<AssetType>().CreateRaw(args...);

	}
	template<typename AssetType, typename... Args> inline AssetType* InsertAssetRaw(const UUID& uuid, Args&&... args) {

		CUP_FUNCTION();

		return GetAssetMap<AssetType>().InsertRaw(uuid, args...);

	}

	template<typename AssetType> inline AssetType* GetAssetRaw(const UUID& uuid) {

		CUP_FUNCTION();

		return GetAssetMap<AssetType>().GetRaw(uuid);

	}

}