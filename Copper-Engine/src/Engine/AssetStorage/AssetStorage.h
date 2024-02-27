#pragma once

#include "Engine/Core/Core.h"

#include "Engine/AssetStorage/AssetList.h"

namespace Copper::AssetStorage {

	template<typename T> AssetList<T>& GetAssetList();

	template<typename T, typename... Args> T* CreateAsset(Args&&... args) { CUP_FUNCTION(); return GetAssetList<T>().Create(args...); }
	template<typename T> T* GetAsset(uint32 index) { CUP_FUNCTION(); return GetAssetList<T>().Get(index); }
	template<typename T> void DeleteAsset(uint32 index) { CUP_FUNCTION(); GetAssetList<T>().Remove(index); }

	template<typename T> uint32 GetAssetIndex(T* asset) { CUP_FUNCTION(); return GetAssetList<T>().GetIndex(asset); }

}