#pragma once

#include "Copper.h"

#include "Panels/Panel.h"

namespace Editor {

    class Properties : public Panel {

    public:
        Properties() : Panel("Properties") {}

        union SelectedData {

            Copper::fs::path file;
            Copper::Entity entity;

            ~SelectedData() {}

        };
        enum class SelectedDataType : Copper::uint8 {

            Entity = 0,
            File,

            None,

        };

        static const SelectedData& GetSelectedData() { return m_selectedData; }
        static SelectedDataType GetSelectedDataType() { return m_selectedDataType; }

        static void SetSelectedEntity(Copper::Entity entity) {

            m_selectedData.entity = entity;
            m_selectedDataType = SelectedDataType::Entity;

        }
        static void SetSelectedFile(const Copper::fs::path& file) {

            m_selectedData.file = file;
            m_selectedDataType = SelectedDataType::File;

        }

        static void ClearSelectedData() { m_selectedDataType = SelectedDataType::None; }
            
    private:
        static SelectedData m_selectedData;
        static SelectedDataType m_selectedDataType;

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
