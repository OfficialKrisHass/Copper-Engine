#pragma once

#include "Copper.h"
#include "Panels/Panel.h"

namespace Editor {

	class SceneHierarchy : public Panel {

	public:
		SceneHierarchy() : Panel("SceneHierarchy") {}
		
		void SetSelectedObject(Copper::Object object) { selectedObj = object; }

		Copper::Object GetSelectedObject() { return selectedObj; }

	private:
		Copper::Object selectedObj;
		Copper::Scene* scene;
		bool anyNodeHovered = false;

		virtual void UI() override;

		void DrawObjectNode(Copper::Object obj);

		bool CheckDropType(std::string wantedType, std::string payloadType) { return payloadType == wantedType; }

		void DropModel(void* data);
		void DropObject(void* data);

	};

}