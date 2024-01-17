#pragma once

#include "Copper.h"

#include "Core/SceneMeta.h"

#include "Panels/Panel.h"

namespace Editor {

	class SceneHierarchy : public Panel {

	public:
		SceneHierarchy();
		
		//Getters
		inline Copper::Entity* GetSelectedEntity() { return &selectedEntity; }

		//Setters
		inline void SetSelectedEntity(Copper::InternalEntity* entity) { selectedEntity = entity; }
		inline void SetScene(Copper::Scene* scene) {
			
			this->scene = scene;
			this->selectedEntity = nullptr;
		
		}

	private:
		Copper::Entity selectedEntity = nullptr;
		Copper::Scene* scene = nullptr;

		virtual void UI() override;

		void DrawEntityNode(Copper::InternalEntity* entity);
		void PopupWindow();

		void RemoveParentTarget();

	};

}