#pragma once

#include "Engine/Components/Camera.h"

namespace Editor {

	class SceneCamera : public Copper::Camera {

	public:
		SceneCamera() = default;
		SceneCamera(Copper::UVector2I size) : Camera(size) {}

		void Update();
		
		inline void SetCanLook(bool can) {this->canLook = can;}
		
		float speed = 0.1f;
		float sensitivity = 100.0f;
	protected:
		bool firstClick = true;
		bool canLook;

	};

}