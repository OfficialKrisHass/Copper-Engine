#include "SceneCamera.h"

#include "Engine/Core/Engine.h"

#include <GLM/glm.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtx/vector_angle.hpp>

namespace Copper {

	void SceneCamera::Update() {
		
		if(!canLook) return;

		if (Input::IsButton(Input::Button2)) {

			if (Input::IsKey(KeyCode::W)) { transform->position +=  transform->forward * speed; }
			if (Input::IsKey(KeyCode::S)) { transform->position += -transform->forward * speed; }
			if (Input::IsKey(KeyCode::A)) { transform->position += -transform->right * speed; }
			if (Input::IsKey(KeyCode::D)) { transform->position +=  transform->right * speed; }
		
			if (Input::IsKey(KeyCode::Space))       { transform->position +=  up * speed; }
			if (Input::IsKey(KeyCode::LeftControl)) { transform->position += -up * speed; }
			
			Input::SetCursorVisible(false);

			if (firstClick) {

				Input::SetCursorPosition((float) GetWindow().Width() / 2, (float) GetWindow().Height() / 2);

				firstClick = false;

			}

			double mouseX;
			double mouseY;
			
			Input::GetCursorPosition(&mouseX, &mouseY);
			
			float rotX = sensitivity * (float) (mouseY - (GetWindow().Height() / 2)) / GetWindow().Height();
			float rotY = sensitivity * (float) (mouseX - (GetWindow().Width() / 2)) / GetWindow().Width();

			transform->rotation.x += rotX;
			transform->rotation.y += rotY;
			
			Input::SetCursorPosition((float) GetWindow().Width() / 2, (float) GetWindow().Height() / 2);

		} else {

			Input::SetCursorVisible(true);
			firstClick = true;

		}

	}

}