#include "SceneCamera.h"

#include "Engine/Core/Engine.h"
#include "Engine/Core/Window.h"

#include <GLM/glm.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtx/vector_angle.hpp>

using namespace Copper;

namespace Editor {

	float rotX;
	float rotY;

	SceneCamera::SceneCamera(UVector2I size) : Camera(size) {

		this->transform = new Transform(Vector3::zero, Vector3::zero, Vector3::one);

	}

	void SceneCamera::Update() {
		
		transform->Update();

		if(!canLook) return;

		if (Input::IsButton(MouseCode::Button2)) {

			if (Input::IsKey(KeyCode::W)) { transform->position +=  transform->Forward() * speed; }
			if (Input::IsKey(KeyCode::S)) { transform->position += -transform->Forward() * speed; }
			if (Input::IsKey(KeyCode::A)) { transform->position += -transform->Right() * speed; }
			if (Input::IsKey(KeyCode::D)) { transform->position +=  transform->Right() * speed; }
		
			if (Input::IsKey(KeyCode::Space))       { transform->position +=  transform->Up() * speed; }
			if (Input::IsKey(KeyCode::LeftControl)) { transform->position += -transform->Up() * speed; }
			
			Input::SetCursorVisible(false);

			if (firstClick) {

				Input::SetCursorPosition((float) GetWindow().Width() / 2, (float) GetWindow().Height() / 2);

				firstClick = false;

			}

			double mouseX;
			double mouseY;
			
			Input::GetCursorPosition(&mouseX, &mouseY);
			
			rotX -= sensitivity * (float) (mouseY - (GetWindow().Height() / 2)) / GetWindow().Height();
			rotY -= sensitivity * (float) (mouseX - (GetWindow().Width() / 2)) / GetWindow().Width();
			transform->rotation = Quaternion(rotX, rotY, 0.0f);
			
			Input::SetCursorPosition((float) GetWindow().Width() / 2, (float) GetWindow().Height() / 2);

		} else {

			Input::SetCursorVisible(true);
			firstClick = true;

		}

	}

}