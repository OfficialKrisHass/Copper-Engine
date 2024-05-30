#pragma once

#include "Copper.h"

#include "Panels/Panel.h"

namespace Editor {

	class Properties : public Panel {

	public:
		Properties();

		static inline void SetSelectedEntity(Copper::Entity entity) {
			
			m_selectedEntity = entity;
			m_selectedFile = "";

		}
		static inline void SetSelectedFile(const Copper::fs::path& path) {
			
			m_selectedFile = path;
			m_selectedEntity = nullptr;
		
		}

		static inline const Copper::fs::path& GetSelectedFile() { return m_selectedFile; }
	
	private:
		static Copper::Entity m_selectedEntity;
		static Copper::fs::path m_selectedFile;

		virtual void UI() override;

		void RenderEntity();
		void RenderFile();

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

		void RenderMaterial(const Copper::MaterialAsset& material);

	};

}
