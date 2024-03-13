#pragma once

#include <Copper.h>

namespace Editor {

	class AssetMeta {

	public:
		AssetMeta() = default;
		AssetMeta(const Copper::fs::path& path, const Copper::UUID& assetUUID) : m_path(path), m_uuid(assetUUID) {}

		static void Serialize(const Copper::fs::path& path, const Copper::UUID& assetUUID);
		bool Deserialize(const Copper::fs::path& path);

		inline void Serialize() const { Serialize(m_path, m_uuid); }

		inline const Copper::UUID& AssetUUID() const { return m_uuid; }

	private:
		Copper::fs::path m_path = "";

		Copper::UUID m_uuid = Copper::UUID("");

	};

}