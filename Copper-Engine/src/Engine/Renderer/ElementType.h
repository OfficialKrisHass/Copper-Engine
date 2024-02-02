#pragma once

#include "Engine/Core/Core.h"

// Wrapper for the data types used in the renderer

namespace Copper {

	enum class ElementType : uint32 {

		Int, Float, Bool,

		Vec2, Vec3, Vec4,
		Vec2I, Vec3I, Vec4I,

	};

	constexpr uint32 TypeSize(ElementType type) {

		switch (type) {

		case ElementType::Int:
		case ElementType::Float:	return 4;
		case ElementType::Bool:		return 1;

		case ElementType::Vec2I:
		case ElementType::Vec2:		return 4 * 2;
		case ElementType::Vec3I:
		case ElementType::Vec3:		return 4 * 3;
		case ElementType::Vec4I:
		case ElementType::Vec4:		return 4 * 4;

		}

		return 0;

	}
	constexpr uint32 TypeDimensions(ElementType type) {

		switch (type) {

		case ElementType::Int:
		case ElementType::Float:
		case ElementType::Bool:		return 1;

		case ElementType::Vec2I:
		case ElementType::Vec2:		return 2;
		case ElementType::Vec3I:
		case ElementType::Vec3:		return 3;
		case ElementType::Vec4I:
		case ElementType::Vec4:		return 4;

		}

		return 0;

	}

}