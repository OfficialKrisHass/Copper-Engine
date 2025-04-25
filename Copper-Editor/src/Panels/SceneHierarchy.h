#pragma once

#include <Copper.h>

#include "Panels/Panel.h"

namespace Editor {

    class SceneHierarchy : public Panel {

    public:
        SceneHierarchy() : Panel("Scene Hierarchy") {}
        
        // Setters

        static void SetScene(Copper::Scene* scene);

    private:
        static Copper::Scene* m_scene;

        Copper::Entity m_entityToRemove;

        virtual void UI() override;

        void DrawEntityNode(Copper::InternalEntity* entity);

        void CommonPopup(Copper::Entity& newEntity);
        void EntityPopup(Copper::Entity entity);
        void PopupWindow();

        void RemoveParentTarget();

    };

}
