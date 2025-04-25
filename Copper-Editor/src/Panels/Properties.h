#pragma once

#include <Copper.h>

#include "Core/SelectedData.h"

#include "Panels/Panel.h"

namespace Copper {

    class Light;
    class MeshRenderer;

    class BoxCollider;
    class CapsuleCollider;
    class SphereCollider;
    class RigidBody;

}

namespace Editor {

    class Properties : public Panel {

    public:
        Properties() : Panel("Properties") {}

        static const SelectedData& GetSelectedData() { return m_selectedData; }

        static void SetSelectedEntity(Copper::Entity entity) {

            m_selectedData.entity = entity;
            m_selectedData.type = SelectedData::Type::Entity;

        }
        static void SetSelectedFile(const Copper::fs::path& file) {

            new(&m_selectedData.file) fs::path();
            m_selectedData.file = file;
            m_selectedData.type = SelectedData::Type::File;

        }

        static void ClearSelectedData() { m_selectedData.type = SelectedData::Type::None; }
            
    private:
        static SelectedData m_selectedData;

        virtual void UI() override;

        void RenderEntity();
        void RenderFile();

        void SerializeFile();

        // Components

        void RenderMeshRenderer(Copper::MeshRenderer* renderer);
        void RenderLight(Copper::Light* light);
        void RenderCamera(Copper::Camera* camera);

        void RenderRigidBody(Copper::RigidBody* rb);

        void RenderBoxCollider(Copper::BoxCollider* collider);
        void RenderSphereCollider(Copper::SphereCollider* collider);
        void RenderCapsuleCollider(Copper::CapsuleCollider* collider);

        void RenderScriptComponent(Copper::ScriptComponent* scriptComponent);

        // Assets

        void RenderMaterial();

    };

}
