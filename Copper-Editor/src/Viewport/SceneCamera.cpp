#include "SceneCamera.h"

#include "Engine/Core/Engine.h"

#include <GLM/glm.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtx/vector_angle.hpp>

namespace Copper {

	void SceneCamera::Update() {
		
		if(!canLook) return;

		if (IsKey(KeyCode::W)) { transform->position += speed *  forward; }
		if (IsKey(KeyCode::S)) { transform->position += speed * -forward; }
		if (IsKey(KeyCode::A)) { transform->position += speed * -glm::normalize(glm::cross((glm::vec3) forward, (glm::vec3) up)); }
		if (IsKey(KeyCode::D)) { transform->position += speed *  glm::normalize(glm::cross((glm::vec3) forward, (glm::vec3) up)); }

		if (IsKey(KeyCode::Space))       { transform->position += speed *  up; }
		if (IsKey(KeyCode::LeftControl)) { transform->position += speed * -up; }

		if (IsButton(Button1)) {

			SetCursorVisible(false);

			if (firstClick) {

				SetCursorPosition((float) GetWindow().Width() / 2, (float) GetWindow().Height() / 2);

				firstClick = false;

			}

			double mouseX;
			double mouseY;
			
			GetCursorPosition(&mouseX, &mouseY);
			
			float rotX = sensitivity * (float) (mouseY - (GetWindow().Height() / 2)) / GetWindow().Height();
			float rotY = sensitivity * (float) (mouseX - (GetWindow().Width() / 2)) / GetWindow().Width();
			
			glm::vec3 newOrientation = glm::rotate((glm::vec3) forward, glm::radians(-rotX), glm::normalize(glm::cross((glm::vec3) forward, (glm::vec3)up)));
			
			if (abs(glm::angle(newOrientation, (glm::vec3) up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {

				forward.x = newOrientation.x;
				forward.y = newOrientation.y;
				forward.z = newOrientation.z;

			}
			
			glm::vec3 vec = glm::rotate((glm::vec3)forward, glm::radians(-rotY), (glm::vec3) up);

			forward.x = vec.x;
			forward.y = vec.y;
			forward.z = vec.z;
			
			SetCursorPosition((float) GetWindow().Width() / 2, (float) GetWindow().Height() / 2);

		} else {

			SetCursorVisible(true);
			firstClick = true;

		}

	}

}