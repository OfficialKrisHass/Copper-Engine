#pragma once

#include "Engine/Core/Core.h"

#include "Engine/AssetStorage/AssetPtr.h"

#include "Engine/Renderer/Texture.h"

namespace Copper {

	class Material {

	public:
		Material() = default;

		TextureAsset texture;
		Color albedo = Color::white;
		float tiling = 1.0f;
		
		static const MaterialAsset& WhiteMaterial();

		inline operator bool() const { return texture; }

	};

}