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

		this->m_transform = new Transform(Vector3::zero, Vector3::zero, Vector3::one);

	}

	void SceneCamera::Update() {
		
		m_transform->Update();

		if(!m_canLook) return;

		if (Input::IsButton(MouseCode::Button2)) {

			if (Input::IsKey(KeyCode::W)) { m_transform->AddPosition( m_transform->Forward() * speed); }
			if (Input::IsKey(KeyCode::S)) { m_transform->AddPosition(-m_transform->Forward() * speed); }
			if (Input::IsKey(KeyCode::A)) { m_transform->AddPosition(-m_transform->Right() * speed); }
			if (Input::IsKey(KeyCode::D)) { m_transform->AddPosition( m_transform->Right() * speed); }
		
			if (Input::IsKey(KeyCode::Space))       { m_transform->AddPosition( m_transform->Up() * speed); }
			if (Input::IsKey(KeyCode::LeftControl)) { m_transform->AddPosition(-m_transform->Up() * speed); }
			
			Input::SetCursorVisible(false);

			if (m_firstClick) {

				Input::SetCursorPosition((float) GetWindow().Width() / 2, (float) GetWindow().Height() / 2);

				m_firstClick = false;

			}

			double mouseX;
			double mouseY;
			
			Input::GetCursorPosition(&mouseX, &mouseY);
			
			rotX -= sensitivity * (float) (mouseY - (GetWindow().Height() / 2)) / GetWindow().Height();
			rotY -= sensitivity * (float) (mouseX - (GetWindow().Width() / 2)) / GetWindow().Width();
			m_transform->SetRotation(Quaternion(rotX, rotY, 0.0f));
			
			Input::SetCursorPosition((float) GetWindow().Width() / 2, (float) GetWindow().Height() / 2);

		} else {

			Input::SetCursorVisible(true);
			m_firstClick = true;

		}

	}

}