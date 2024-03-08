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

		void RenderScriptComponent(Copper::ScriptComponent* script);

		template<typename T, typename F> void RenderScriptField(Copper::ScriptComponent* script, const Copper::ScriptField& field, F showFunc);

		// Assets

		void RenderMaterial(const Copper::Material& material);

	};

}