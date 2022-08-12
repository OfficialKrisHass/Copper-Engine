#pragma once

#include "Copper.h"
#include "Panels/Panel.h"

namespace Editor {

	class SceneHierarchy : public Panel {

	public:
		SceneHierarchy() : Panel("SceneHierarchy") {}

		virtual void UIRender() override;

	private:
		//

	};

}