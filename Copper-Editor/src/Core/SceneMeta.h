#pragma once

#include <Copper.h>

namespace Editor::MetaFile {

	struct SceneMeta {

	public:
		void Serialize(Copper::Scene* scene);
		void Deserialize(Copper::Scene* scene);

		std::vector<uint32_t> objectIDs;

	};

}