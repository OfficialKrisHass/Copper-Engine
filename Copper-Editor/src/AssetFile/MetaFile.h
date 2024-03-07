#pragma once

#include <Copper.h>

namespace Editor {

	class MetaFile {

	public:
		MetaFile() = default;
		MetaFile(const Copper::fs::path& path, const Copper::UUID& assetUUID) : m_path(path), m_uuid(assetUUID) {}

		void Serialize();
		bool Deserialize(const Copper::fs::path& path);

		static void Serialize(const Copper::fs::path& path, const Copper::UUID& assetUUID);

		inline const Copper::UUID& AssetUUID() const { return m_uuid; }

	private:
		Copper::fs::path m_path = "";

		Copper::UUID m_uuid = Copper::UUID("");

	};

}