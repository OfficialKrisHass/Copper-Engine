#pragma once

#include "Copper.h"
#include "Panels/Panel.h"

#include "Engine/Scene/Scene.h"

namespace Editor {

	class SceneHierarchy : public Panel {

	public:
		SceneHierarchy() : Panel("SceneHierarchy") {}

		virtual void UIRender() override;

		inline void SetScene(Copper::Scene* scene) { this->scene = scene; }

	private:
		Copper::Scene* scene = nullptr;
		Copper::Object selectedObj = { Copper::ECS::EntityID(-1, -1), nullptr };

	};

}