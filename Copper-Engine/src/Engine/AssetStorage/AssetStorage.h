#pragma once

#include "Engine/Core/Core.h"

#include "Engine/AssetStorage/AssetList.h"

namespace Copper::AssetStorage {

	template<typename T> AssetList<T>& GetAssetList();

	template<typename T, typename... Args> T* CreateAsset(Args&&... args) { return GetAssetList<T>().Create(args...); }
	template<typename T> T* GetAsset(uint32 index) { return GetAssetList<T>().Get(index); }
	template<typename T> void DeleteAsset(uint32 index) { GetAssetList<T>().Remove(index); }

}