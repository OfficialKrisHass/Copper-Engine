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

        m_transform = new Transform(Vector3::zero, Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vector3::one);

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

                Input::SetCursorPosition((float) GetWindow().GetWidth() / 2, (float) GetWindow().GetHeight() / 2);

                m_firstClick = false;

            }

            double mouseX;
            double mouseY;

            Input::GetCursorPosition(&mouseX, &mouseY);

            rotX -= sensitivity * (float) (mouseY - (GetWindow().GetHeight() / 2)) / GetWindow().GetHeight();
            rotY -= sensitivity * (float) (mouseX - (GetWindow().GetWidth() / 2)) / GetWindow().GetWidth();
            m_transform->SetRotation(Quaternion(rotX, rotY, 0.0f));

            Input::SetCursorPosition((float) GetWindow().GetWidth() / 2, (float) GetWindow().GetHeight() / 2);

        } else {

            Input::SetCursorVisible(true);
            m_firstClick = true;

        }

    }

}
