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
		inline Copper::Object& GetSelectedObject() { return selectedObj; }

		static MetaFile::SceneMeta* GetSceneMetaPointer();

		//Setters
		inline void SetSelectedObject(Copper::Object& object) { selectedObj = object; }
		inline void SetScene(Copper::Scene* scene) { this->scene = scene; }

	private:
		Copper::Object selectedObj;
		Copper::Scene* scene;

		virtual void UI() override;

		void DrawObjectNode(uint32_t& objIDIndex);
		void RemoveObjectNode(uint32_t objIDIndex);
		void MoveObjectNode(uint32_t& objIDIndex, uint32_t objToMoveIDIndex);

		void PopupWindow();

		void RemoveParent(uint32_t objIDIndex);

		bool OnObjectCreated(const Copper::Event& e);
		bool OnObjectDestroyed(const Copper::Event& e);

	};

}