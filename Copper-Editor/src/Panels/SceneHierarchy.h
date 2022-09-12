#pragma once

#include "Copper.h"
#include "Panels/Panel.h"

#include "Engine/Scene/Scene.h"

namespace Editor {

	class SceneHierarchy : public Panel {

	public:
		SceneHierarchy() : Panel("SceneHierarchy") {}

		inline void SetScene(Copper::Scene* scene) { this->scene = scene; }

		inline Copper::Object GetSelectedObject() { return selectedObj; }

	private:
		Copper::Scene* scene = nullptr;
		Copper::Object selectedObj = { Copper::ECS::EntityID(-1, -1), nullptr };

		virtual void UI() override;

	};

}