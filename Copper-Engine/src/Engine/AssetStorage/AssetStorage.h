#pragma once

#include "Engine/Core/Core.h"

#include "Engine/AssetStorage/AssetMap.h"

namespace Copper::AssetStorage {

	template<typename AssetType> AssetMap<AssetType>& GetAssetMap();

	template<typename AssetType, typename... Args> AssetPtr<AssetType> CreateAsset(Args&&... args) {
		
		CUP_FUNCTION();
		return GetAssetMap<AssetType>().Create(args...);

	}
	template<typename AssetType, typename... Args> AssetPtr<AssetType> InsertAsset(const UUID& uuid, Args&&... args) {

		CUP_FUNCTION();
		return GetAssetMap<AssetType>().Insert(uuid, args...);

	}

	template<typename AssetType> AssetPtr<AssetType> GetAsset(const UUID& uuid) {
		
		CUP_FUNCTION();
		return GetAssetMap<AssetType>().Get(uuid);
	
	}

	template<typename AssetType> void DeleteAsset(const UUID& uuid) {
		
		CUP_FUNCTION();
		GetAssetMap<AssetType>().Remove(uuid);
	
	}
	template<typename AssetType> void DeleteAsset(const AssetPtr<AssetType>& asset) {

		CUP_FUNCTION();
		GetAssetMap<AssetType>().Remove(asset);

	}

	template<typename AssetType, typename... Args> AssetType* CreateAssetRaw(Args&&... args) {

		CUP_FUNCTION();
		return GetAssetMap<AssetType>().CreateRaw(args...);

	}
	template<typename AssetType, typename... Args> AssetType* InsertAssetRaw(const UUID& uuid, Args&&... args) {

		CUP_FUNCTION();
		return GetAssetMap<AssetType>().InsertRaw(uuid, args...);

	}

	template<typename AssetType> AssetType* GetAssetRaw(const UUID& uuid) {

		CUP_FUNCTION();
		return GetAssetMap<AssetType>().GetRaw(uuid);

	}

}