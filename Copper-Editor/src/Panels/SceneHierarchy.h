#pragma once

#include "Copper.h"

#include "Core/SceneMeta.h"

#include "Panels/Panel.h"

namespace Editor {

	class SceneHierarchy : public Panel {

	public:
		SceneHierarchy();
		
		//Getters
		inline Copper::Entity* GetSelectedEntity() { return &m_selectedEntity; }

		//Setters
		inline void SetSelectedEntity(Copper::InternalEntity* entity) { m_selectedEntity = entity; }
		inline void SetScene(Copper::Scene* scene) {
			
			this->m_scene = scene;
			this->m_selectedEntity = nullptr;
		
		}

	private:
		Copper::Entity m_selectedEntity = nullptr;
		Copper::Scene* m_scene = nullptr;

		virtual void UI() override;

		void DrawEntityNode(Copper::InternalEntity* entity);
		void PopupWindow();

		void RemoveParentTarget();

	};

}