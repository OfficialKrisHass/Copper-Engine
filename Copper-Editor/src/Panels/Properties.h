#pragma once

#include "Copper.h"

#include "Panels/Panel.h"

namespace Editor {

	class Properties : public Panel {

	public:
		Properties();

		void SetSelectedObject(Copper::Entity* entity) {
			
			this->m_selectedEntity = entity;
			
		}
	
	private:
		Copper::Entity* m_selectedEntity = nullptr;

		virtual void UI() override;

		void RenderEntity();

		void RenderLight(Copper::Light* light);
		void RenderCamera(Copper::Camera* camera);

		void RenderRigidBody(Copper::RigidBody* rb);

		void RenderBoxCollider(Copper::BoxCollider* collider);
		void RenderSphereCollider(Copper::SphereCollider* collider);
		void RenderCapsuleCollider(Copper::CapsuleCollider* collider);

		void RenderScriptComponent(Copper::ScriptComponent* script);

		template<typename T, typename F> void RenderScriptField(Copper::ScriptComponent* script, const Copper::ScriptField& field, F showFunc);

	};

}