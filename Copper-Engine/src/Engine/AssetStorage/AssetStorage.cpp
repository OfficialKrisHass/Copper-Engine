#include "cupch.h"
#include "AssetStorage.h"

#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Material.h"

namespace Copper::AssetStorage {

	AssetList<Texture> textureAssets;
	AssetList<Material> materialAssets;
	
	template<> AssetList<Texture>& GetAssetList<Texture>() { return textureAssets; }
	template<> AssetList<Material>& GetAssetList<Material>() { return materialAssets; }

}