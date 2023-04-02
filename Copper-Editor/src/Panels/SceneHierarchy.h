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

		/*void DrawObjectNode(uint32_t& objIDIndex);
		void RemoveObjectNode(uint32_t objIDIndex);
		void MoveObjectNode(uint32_t& objIDIndex, uint32_t objToMoveIDIndex);

		void ChangeObjectParent(uint32_t& objIDIndex, uint32_t& newParentIDIndex);

		void RemoveParent(uint32_t objIDIndex);

		bool OnObjectCreated(const Copper::Event& e);
		bool OnObjectDestroyed(const Copper::Event& e);*/

	};

}