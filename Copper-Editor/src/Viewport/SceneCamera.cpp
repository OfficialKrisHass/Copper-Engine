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

        float horizontal = Input::GetAxis("Keys_WS") * speed * m_multiplier * GetDeltaTime();
        float vertical = Input::GetAxis("Keys_DA") * speed * m_multiplier * GetDeltaTime();

        m_transform->AddPosition(m_transform->GetForward() * horizontal + m_transform->GetRight() * vertical);

        if (Input::GetKeyState(KeyCode::Space) == KeyState::Down)
            m_transform->AddPosition( m_transform->GetUp() * speed * GetDeltaTime());
        if (Input::GetKeyState(KeyCode::LeftControl) == KeyState::Down)
            m_transform->AddPosition(-m_transform->GetUp() * speed * GetDeltaTime());

        // Look

        float deltaX = -Input::GetMouseAxis("Mouse X") / GetWindow().GetWidth() * sensitivity * GetDeltaTime();
        float deltaY = -Input::GetMouseAxis("Mouse Y") / GetWindow().GetHeight() * sensitivity * GetDeltaTime();

        m_rotation = Quaternion(Vector3(0.0f, 1.0f, 0.0f) * deltaX) * m_rotation;
        m_rotation *= Quaternion(Vector3(1.0f, 0.0f, 0.0f) * deltaY);
        m_transform->SetRotation(m_rotation);

    }

    void SceneCamera::UpdateRotation() {

        CUP_FUNCTION();

        m_rotation = m_transform->GetRotation();

    }

}
