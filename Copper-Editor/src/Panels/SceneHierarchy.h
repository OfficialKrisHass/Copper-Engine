#pragma once

#include "Copper.h"

#include "Core/MetaFileSerialization.h"

#include "Panels/Panel.h"

namespace Editor {

	class SceneHierarchy : public Panel {

	public:
		SceneHierarchy();

		void SaveSceneMeta();
		void LoadSceneMeta();
		
		//Getters
		inline Copper::Entity* GetSelectedEntity() { return &selectedEntity; }
		static MetaFile::SceneMeta* GetSceneMetaPointer();

		//Setters
		inline void SetSelectedEntity(Copper::Entity entity) { selectedEntity = entity; }
		inline void SetScene(Copper::Scene* scene) { this->scene = scene; }

	private:
		Copper::Entity selectedEntity;
		Copper::Scene* scene = nullptr;

		virtual void UI() override;

		void DrawEntityNode(Copper::InternalEntity* entity);

		/*void DrawObjectNode(uint32_t& objIDIndex);
		void RemoveObjectNode(uint32_t objIDIndex);
		void MoveObjectNode(uint32_t& objIDIndex, uint32_t objToMoveIDIndex);

		void ChangeObjectParent(uint32_t& objIDIndex, uint32_t& newParentIDIndex);

		void PopupWindow();

		void RemoveParent(uint32_t objIDIndex);

		bool OnObjectCreated(const Copper::Event& e);
		bool OnObjectDestroyed(const Copper::Event& e);*/

	};

}