#include "SceneCamera.h"

#include "Engine/Core/Engine.h"

#include <GLM/glm.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtx/vector_angle.hpp>

using namespace Copper;

namespace Editor {

	void SceneCamera::Update() {
		
		GetTransform()->Update();

		if(!canLook) return;

		if (Input::IsButton(MouseCode::Button2)) {

			if (Input::IsKey(KeyCode::W)) { GetTransform()->position +=  GetTransform()->Forward() * speed; }
			if (Input::IsKey(KeyCode::S)) { GetTransform()->position += -GetTransform()->Forward() * speed; }
			if (Input::IsKey(KeyCode::A)) { GetTransform()->position += -GetTransform()->Right() * speed; }
			if (Input::IsKey(KeyCode::D)) { GetTransform()->position +=  GetTransform()->Right() * speed; }
		
			if (Input::IsKey(KeyCode::Space))       { GetTransform()->position +=  GetTransform()->Up() * speed; }
			if (Input::IsKey(KeyCode::LeftControl)) { GetTransform()->position += -GetTransform()->Up() * speed; }
			
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
			
			Vector3 tmp = glm::degrees(glm::eulerAngles((glm::quat) (GetTransform()->rotation)));
			tmp.y -= rotY;
			GetTransform()->rotation = glm::quat(glm::radians((glm::vec3) (tmp)));
			
			Input::SetCursorPosition((float) GetWindow().Width() / 2, (float) GetWindow().Height() / 2);

		} else {

			Input::SetCursorVisible(true);
			firstClick = true;

		}

	}

}