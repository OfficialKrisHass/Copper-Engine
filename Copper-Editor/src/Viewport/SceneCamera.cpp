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

        m_transform = new Transform(Vector3::zero, Quaternion::identity, Vector3::one);

    }

    void SceneCamera::Update() {

        m_transform->Update();

        if(!m_canLook) return;

        CU_ASSERT(m_viewport != nullptr, "SceneCamera was not assigned a viewport!");

        switch (Input::GetKeyState(KeyCode::Mouse1)) {

            case KeyState::None: return;
            case KeyState::Pressed: {

                Input::SetCursorVisible(false);
                Input::SetCursorLocked(true);

                return;

            }
            case KeyState::Down: break;
            case KeyState::Released: {

                Input::SetCursorVisible(true);
                Input::SetCursorLocked(false);

                return;

            }
            default: return;

        }

        // Movement

        float horizontal = Input::GetAxis("Keys_WS") * speed * GetDeltaTime();
        float vertical = Input::GetAxis("Keys_DA") * speed * GetDeltaTime();

        m_transform->AddPosition(m_transform->Forward() * horizontal + m_transform->Right() * vertical);

        if (Input::GetKeyState(KeyCode::Space) == KeyState::Down)
            m_transform->AddPosition( m_transform->Up() * speed * GetDeltaTime());
        if (Input::GetKeyState(KeyCode::LeftControl) == KeyState::Down)
            m_transform->AddPosition(-m_transform->Up() * speed * GetDeltaTime());

        // Look

        m_rotY -= Input::GetMouseAxis("Mouse X") / GetWindow().GetWidth() * sensitivity * GetDeltaTime();
        m_rotX -= Input::GetMouseAxis("Mouse Y") / GetWindow().GetHeight() * sensitivity * GetDeltaTime();
        m_transform->SetRotation(Quaternion(m_rotX, m_rotY, 0.0f));

    }

    void SceneCamera::UpdateRotation() {

        CUP_FUNCTION();

        Vector3 euler = m_transform->Rotation().EulerAngles();
        m_rotX = -euler.x;
        m_rotY = euler.y;

    }

}
