#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/ElementType.h"

#include <glad/glad.h>

namespace Copper {

	constexpr uint32 TypeToOpenGL(ElementType type) {

		switch (type) {

		case ElementType::Float:
		case ElementType::Vec2:
		case ElementType::Vec3:
		case ElementType::Vec4:		return GL_FLOAT;

		case ElementType::Int:
		case ElementType::Vec2I:
		case ElementType::Vec3I:
		case ElementType::Vec4I:	return GL_INT;

		case ElementType::Bool:		return GL_BOOL;

		}

		LogWarn("Couldn't get the OpenGL data type for {} (ElementType)", (uint32) type);
		return 0;

	}

}