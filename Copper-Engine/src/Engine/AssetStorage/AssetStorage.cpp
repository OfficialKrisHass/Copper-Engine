#include "cupch.h"
#include "AssetStorage.h"

#include "Engine/Renderer/Texture.h"

namespace Copper::AssetStorage {

	AssetList<Texture> textureAssets;
	
	template<> AssetList<Texture>& GetAssetList<Texture>() { return textureAssets; }

}