#pragma once

#include "Engine/Core/Core.h"

#include "Engine/AssetStorage/AssetPtr.h"

namespace Copper {

	class MaterialData {

	public:
		MaterialData() = default;

		Texture texture;
		Color albedo = Color::white;
		float tiling = 1.0f;
		
		static const Material WhiteMaterial();

		inline operator bool() const { return texture; }

	};

}