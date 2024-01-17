#pragma once

#include <Copper.h>

namespace Editor::MetaFile {

	struct SceneMeta {

	public:
		void Serialize();
		void Deserialize(Copper::Scene* scene);

	private:
		Copper::Scene* scene;

	};

}