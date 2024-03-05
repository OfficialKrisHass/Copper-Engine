#include "cupch.h"
#include "AssetStorage.h"

#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Material.h"

namespace Copper::AssetStorage {

	AssetMap<Texture> textureMap;
	AssetMap<Material> materialMap;
	
	template<> AssetMap<Texture>& GetAssetMap<Texture>() { return textureMap; }
	template<> AssetMap<Material>& GetAssetMap<Material>() { return materialMap; }

}