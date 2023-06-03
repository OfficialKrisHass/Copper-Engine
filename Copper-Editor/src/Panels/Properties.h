#pragma once

#include "Copper.h"

#include "Panels/Panel.h"

namespace Editor {

	class Properties : public Panel {

	public:
		Properties();

		void SetSelectedObject(Copper::Entity* entity) {
			
			this->selectedEntity = entity;
			
		}

		bool ShowBool(const std::string& name, bool* show);
		bool ShowInt(const std::string& name, int* show);
		bool ShowUInt(const std::string& name, unsigned int* show);
		bool ShowFloat(const std::string& name, float* show);
		bool ShowDouble(const std::string& name, double *show);
		bool ShowString(const std::string& name, std::string* show);
		bool ShowChar(const std::string& name, char* show);

		bool ShowVector2(const std::string& name, Copper::Vector2* vec);
		bool ShowVector3(const std::string& name, Copper::Vector3* vec);
		bool ShowVector4(const std::string& name, Copper::Vector4* vec);
		bool ShowColor(const std::string& name, Copper::Color* col);

		bool ShowEntity(const std::string& name, Copper::InternalEntity** entity);

		static bool IsDragDropTargetHovered() { return dragDropTargetHovered; }
	
	private:
		Copper::Entity* selectedEntity = nullptr;

		static bool dragDropTargetHovered;

		virtual void UI() override;

		void RenderEntity();

		void RenderLight(Copper::Light* light);
		void RenderCamera(Copper::Camera* camera);

		void RenderScriptComponent(Copper::ScriptComponent* script);

		void RenderPhysicsObject(Copper::PhysicsObject* object);

		template<typename T, typename F> void RenderScriptField(Copper::ScriptComponent* script, const Copper::ScriptField& field, F showFunc);

	};

}