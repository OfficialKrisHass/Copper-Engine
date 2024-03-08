#pragma once

#include "Copper.h"

#include "Core/SceneMeta.h"

#include "Panels/Panel.h"

namespace Editor {

	class SceneHierarchy : public Panel {

	public:
		SceneHierarchy();
		
		// Getters
		inline static Copper::Entity GetSelectedEntity() { return m_selectedEntity; }

		// Setters

		inline static void SetScene(Copper::Scene* scene) {
			
			m_scene = scene;
			m_selectedEntity = nullptr;
		
		}
		inline static void SetSelectedEntity(Copper::InternalEntity* entity) { m_selectedEntity = entity; }

	private:
		static Copper::Entity m_selectedEntity;
		static Copper::Scene* m_scene;

		virtual void UI() override;

		void DrawEntityNode(Copper::InternalEntity* entity);
		void PopupWindow();

		void RemoveParentTarget();

	};

}