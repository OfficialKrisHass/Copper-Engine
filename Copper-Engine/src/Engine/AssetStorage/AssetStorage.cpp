#include "cupch.h"
#include "AssetStorage.h"

#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Material.h"

namespace Copper::AssetStorage {

	AssetMap<TextureData> textureMap;
	AssetMap<MaterialData> materialMap;
	
	template<> AssetMap<TextureData>& GetAssetMap<TextureData>() { return textureMap; }
	template<> AssetMap<MaterialData>& GetAssetMap<MaterialData>() { return materialMap; }

}