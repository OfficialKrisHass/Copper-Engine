#pragma once

#include "Copper.h"

#include "Core/EditorApp.h"

#include "Panels/Panel.h"

namespace Editor {

    class SceneHierarchy : public Panel {

    public:
        SceneHierarchy() : Panel("Scene Hierarchy") {}
        
        // Setters

        inline static void SetScene(Copper::Scene* scene) {
            
            m_scene = scene;
            SetSelectedEntity(nullptr);
        
        }

    private:
        static Copper::Scene* m_scene;

        virtual void UI() override;

        void DrawEntityNode(Copper::InternalEntity* entity);
        void PopupWindow();

        void RemoveParentTarget();

    };

}
