#pragma once

#include <Copper.h>

namespace Editor {

	struct SceneMeta {

	public:
		void Serialize(const Copper::fs::path& path);
		void Deserialize(const Copper::fs::path& path);

	};

}
