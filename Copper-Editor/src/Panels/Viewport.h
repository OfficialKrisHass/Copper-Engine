#pragma once

#include <Copper.h>

#include <Engine/Renderer/FrameBuffer.h>

#include "Panels/Panel.h"

#include "Viewport/SceneCamera.h"

namespace Editor {

    using namespace Copper;

    class Viewport : public Panel {

    public:
        struct Gizmo {

            // This is equal to ImGuizmo::OPERATION, just in our naming scheme
            enum Operation : uint16 {

                Translate = FLAG(0) | FLAG(1) | FLAG(2),
                Rotate = FLAG(3) | FLAG(4) | FLAG(5),
                Scale = FLAG(7) | FLAG(8) | FLAG(9),

            };

            Operation operation = Translate;
            bool globalMode = false;

            bool active = false;

            Vector3 savedPosition;
            Quaternion savedRotation;
            Vector3 savedScale;

        };

        Viewport() : Panel("Viewport") {}

        void Initialize();
        void Update();
        virtual void UIRender() override;

        inline const Gizmo& GetGizmo() const { return m_gizmo; }
        inline SceneCamera& GetSceneCamera() { return m_sceneCam; }

        // TODO: Remove this
        inline const UVector2I& GetCentre() const { return m_centre; }

        inline void SetGizmoOperation(Gizmo::Operation value) { m_gizmo.operation = value; }
        inline void SetGizmoGlobalMode(bool value) { m_gizmo.globalMode = value; }

    private:
        UVector2I m_size = UVector2I(1280, 720);
        UVector2I m_centre;
        Vector2I m_mousePos;

        SceneCamera m_sceneCam; 
        FrameBuffer m_fbo;

        bool m_firstFrame = true;

        Gizmo m_gizmo;

        virtual void UI() override { }


    };

}
