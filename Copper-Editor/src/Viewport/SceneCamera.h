#pragma once

#include "Engine/Components/Camera.h"

namespace Copper {

	class SceneCamera : public Camera {

	public:
		SceneCamera() = default;
		SceneCamera(UVector2I size) : Camera(size) {}

		virtual void Update() override;
		
		inline void SetCanLook(bool can) {this->canLook = can;}
		
		float speed = 0.1f;
		float sensitivity = 100.0f;
	protected:
		bool firstClick = true;
		bool canLook;

	};

}