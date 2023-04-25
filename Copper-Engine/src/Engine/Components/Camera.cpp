#include "cupch.h"
#include "Camera.h"

#include "Engine/Core/Engine.h"

#include "Engine/Components/Transform.h"

#include <GLM/ext/matrix_clip_space.hpp>
#include <GLM/ext/matrix_transform.hpp>

namespace Copper {

	Matrix4 Camera::CreateViewMatrix() {

		return CMath::ViewMatrix(GetTransform()->position, GetTransform()->Forward(), GetTransform()->Up());

	}
	Matrix4 Camera::CreateProjectionMatrix() {

		Matrix4 ret;

	#ifdef CU_EDITOR
		ret = CMath::ProjectionMatrix(fov, static_cast<float>(size.x) / size.y, nearPlane, farPlane);
	#else
		ret = CMath::ProjectionMatrix(fov, GetWindowAspectRatio(), nearPlane, farPlane);
	#endif

		return ret;

	}

}