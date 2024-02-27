#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Texture.h"

namespace Copper {

	class Material {

	public:
		Material() = default;

		Texture* texture = nullptr;
		Color albedo = Color::white;
		float tiling = 1.0f;

		static const Material* WhiteMaterial();

		inline operator bool() const { return texture; }

	};

}