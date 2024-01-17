#pragma once

#include "Copper.h"

namespace Editor {

	class SceneCamera : public Copper::Camera {

		friend struct Project;

	public:
		SceneCamera() = default;
		SceneCamera(Copper::UVector2I size);

		void Update();
		
		inline void SetCanLook(bool can) { this->canLook = can; }
		
		float speed = 0.1f;
		float sensitivity = 100.0f;

	private:
		bool firstClick = true;
		bool canLook = false;

	};

}