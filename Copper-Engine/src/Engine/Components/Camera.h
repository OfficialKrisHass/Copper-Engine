#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

#ifdef CU_EDITOR
namespace Editor { struct Project; }
#endif

namespace Copper {

	class Camera : public Component {

		friend class Scene;
	#ifdef CU_EDITOR
		friend Editor::Project;
	#endif

	public:
		Camera() = default;
		Camera(const UVector2I& size) : size(size) {}

		float fov = 90.0f;
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;

		Matrix4 CreateViewMatrix() const;
		Matrix4 CreateProjectionMatrix() const;

		inline void Resize(const UVector2I& size) { this->size = size; }

	protected:
	#ifdef CU_EDITOR
		UVector2I size = UVector2I(1280, 720);
	#endif


	};

}