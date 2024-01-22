#pragma once

#include "Copper.h"

namespace Editor {

	class SceneCamera : public Copper::Camera {

		friend struct Project;

	public:
		SceneCamera() = default;
		SceneCamera(Copper::UVector2I size);

		void Update();
		
		inline void SetCanLook(bool can) { this->m_canLook = can; }
		
		float speed = 0.1f;
		float sensitivity = 100.0f;

	private:
		bool m_firstClick = true;
		bool m_canLook = false;

	};

}