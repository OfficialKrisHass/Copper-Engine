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

		bool ShowBool(const std::string& name, bool* show);
		bool ShowInt(const std::string& name, Copper::int32* show);
		bool ShowUInt(const std::string& name, Copper::uint32* show);
		bool ShowFloat(const std::string& name, float* show);
		bool ShowDouble(const std::string& name, double *show);
		bool ShowString(const std::string& name, std::string* show);
		bool ShowChar(const std::string& name, char* show);

		bool ShowVector2(const std::string& name, Copper::Vector2* vec);
		bool ShowVector3(const std::string& name, Copper::Vector3* vec);
		bool ShowVector4(const std::string& name, Copper::Vector4* vec);
		bool ShowColor(const std::string& name, Copper::Color* col);

		bool ShowEntity(const std::string& name, Copper::InternalEntity** entity);
		bool ShowTransform(const std::string& name, Copper::Transform** transform);

		bool ShowDropDown(const std::string& name, const char* items[], Copper::uint32 count, Copper::uint32* selected);
		bool ShowMask(const std::string& name, Copper::uint32& mask, Copper::uint32 num, Copper::uint32 maskOffset = 0, char startLabel = 'X');
	
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