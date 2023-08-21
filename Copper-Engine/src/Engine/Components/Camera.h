#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

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

		Matrix4 CreateViewMatrix();
		Matrix4 CreateProjectionMatrix();

		void Resize(const UVector2I& size) { this->size = size; }

		float fov = 90.0f;
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;

	protected:
		UVector2I size = UVector2I(1280, 720);


	};

}