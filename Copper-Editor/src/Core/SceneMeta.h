#pragma once

#include <Copper.h>

namespace Editor {

	struct SceneMeta {

	public:
		void Serialize();
		void Deserialize(Copper::Scene* scene);

	private:
		Copper::Scene* m_scene;

	};

}