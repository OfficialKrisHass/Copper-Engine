#include "cupch.h"
#include "Camera.h"

#include "Engine/Core/Engine.h"

#include "Engine/Components/Transform.h"

namespace Copper {

	Matrix4 Camera::CreateViewMatrix() const {

		return CMath::ViewMatrix(GetTransform()->GlobalPosition(), GetTransform()->Forward(), GetTransform()->Up());

	}
	Matrix4 Camera::CreateProjectionMatrix() const {

	#ifdef CU_EDITOR
		Matrix4 ret = CMath::ProjectionMatrix(fov, static_cast<float>(size.x) / size.y, nearPlane, farPlane);
	#else
		Matrix4 ret = CMath::ProjectionMatrix(fov, GetWindowAspectRatio(), nearPlane, farPlane);
	#endif

		return ret;

	}

}