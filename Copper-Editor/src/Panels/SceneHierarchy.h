#pragma once

#include "Copper.h"
#include "Panels/Panel.h"

#include "Engine/Scene/Scene.h"

namespace Editor {

	class SceneHierarchy : public Panel {

	public:
		SceneHierarchy() : Panel("SceneHierarchy") {}
		
		void SetSelectedObject(Copper::Object object) { selectedObj = object; }

		Copper::Object GetSelectedObject() { return selectedObj; }

	private:
		Copper::Object selectedObj = Copper::Object::Null();

		virtual void UI() override;

	};

}