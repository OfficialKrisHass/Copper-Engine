#include "cupch.h"
#include "AssetStorage.h"

#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Mesh.h"

namespace Copper::AssetStorage {

	AssetMap<Texture> textureMap;
	AssetMap<Material> materialMap;
	AssetMap<Mesh> meshMap;
	
	template<> AssetMap<Texture>& GetAssetMap<Texture>() { return textureMap; }
	template<> AssetMap<Material>& GetAssetMap<Material>() { return materialMap; }
	template<> AssetMap<Mesh>& GetAssetMap<Mesh>() { return meshMap; }

}