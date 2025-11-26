#include "SceneCamera.h"

#include "Panels/Viewport.h"

#include <Engine/Core/Window.h>

#include <Engine/Input/Input.h>
#include <Engine/Input/AxisManager.h>

#include <GLM/glm.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <GLM/gtx/vector_angle.hpp>

using namespace Copper;

namespace Editor {

    SceneCamera::SceneCamera(UVector2I size, Viewport* viewport) : Camera(size), m_viewport(viewport) {

        CUP_FUNCTION();

        m_transform = new Transform();

    }

    void SceneCamera::Update() {

        CUP_FUNCTION();

        CU_ASSERT(m_viewport != nullptr, "SceneCamera was not assigned a viewport!");
        CU_ASSERT(m_transform != nullptr, "SceneCamera doesn't have a transform!");

        if(!m_canLook) return;

        switch (Input::GetKeyState(KeyCode::Mouse1)) {

            case KeyState::None: return;
            case KeyState::Pressed: {

                Input::SetCursorVisible(false);
                Input::SetCursorLocked(true);

                return;

            }
            case KeyState::Down: {

                if (Input::GetCursorVisible()) return;
                break;

            }
            case KeyState::Released: {

                Input::SetCursorVisible(true);
                Input::SetCursorLocked(false);

                return;

            }
            default: return;

        }

        if (Input::GetKeyState(KeyCode::LeftShift) == KeyState::Pressed)
            m_multiplier = 2.0f;
        else if (Input::GetKeyState(KeyCode::LeftShift) == KeyState::Released)
            m_multiplier = 1.0f;

        // Movement

        float multiplier = speed * m_multiplier * GetDeltaTime() * 10.0f; // Incredibly slow without the times 10
        float horizontal = Input::GetAxis("Keys_WS") * multiplier;
        float vertical = Input::GetAxis("Keys_AD") * multiplier;

        m_transform->AddPosition(m_transform->GetForward() * horizontal + m_transform->GetRight() * vertical);

        if (Input::GetKeyState(KeyCode::E) == KeyState::Down)
            m_transform->AddPosition( m_transform->GetUp() * multiplier);
        if (Input::GetKeyState(KeyCode::Q) == KeyState::Down)
            m_transform->AddPosition(-m_transform->GetUp() * multiplier);

        // Look

        multiplier = sensitivity * GetDeltaTime() * 5000.0f; // Incredibly slow without the times 5000
        float deltaX = -Input::GetMouseAxis("Mouse X") / GetWindow().GetWidth() * multiplier;
        float deltaY = -Input::GetMouseAxis("Mouse Y") / GetWindow().GetHeight() * multiplier;

        m_rotation = Quaternion(Vector3(0.0f, 1.0f, 0.0f) * deltaX) * m_rotation;
        m_rotation *= Quaternion(Vector3(1.0f, 0.0f, 0.0f) * deltaY);
        m_transform->SetRotation(m_rotation);

    }

    void SceneCamera::UpdateRotation() {

        CUP_FUNCTION();

        m_rotation = m_transform->GetRotation();

    }

}
