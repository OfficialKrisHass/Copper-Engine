#include "MetaFile.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Editor {

	using namespace Copper;

	void MetaFile::Serialize() {

		CUP_FUNCTION();
		Serialize(m_path, m_uuid);

	}
	bool MetaFile::Deserialize(const fs::path& path) {

		CUP_FUNCTION();
		m_path = path;

		YAML::Node node;
		try { node = YAML::LoadFile(path.string()); }
		catch (YAML::Exception e) {

			LogError("Couldn't Load Meta file!\n\tPath: {}", path.string());
			m_uuid = UUID("");

			return false;

		}

		m_uuid = node["UUID"].as<UUID>();

		return true;

	}

	void MetaFile::Serialize(const fs::path& path, const UUID& assetUUID) {

		CUP_FUNCTION();

		YAML::Emitter out;
		out << YAML::BeginMap; // Main

		out << YAML::Key << "UUID" << YAML::Value << assetUUID;

		out << YAML::EndMap; // Main

		std::ofstream file(path);
		file << out.c_str();
		file.close();

	}

}