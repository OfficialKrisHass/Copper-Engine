#pragma once

#include <Copper.h>

namespace Editor::MetaFile {

	struct SceneMeta {

	public:

		void Serialize();
		void Deserialize(Copper::Scene* scene);

		std::vector<uint32_t> objectIDs;

		Copper::InternalEntity* GetEntity(uint32_t index) const;

	private:
		Copper::Scene* scene;

		void InitDefault();

	};

}