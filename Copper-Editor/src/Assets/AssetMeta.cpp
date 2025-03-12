#include "AssetMeta.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Editor {

    using namespace Copper;

    void AssetMeta::Serialize(const fs::path& path, const UUID& assetUUID) {

        CUP_FUNCTION();

        YAML::Emitter out;
        out << YAML::BeginMap; // Main

        out << YAML::Key << "UUID" << YAML::Value << assetUUID;

        out << YAML::EndMap; // Main

        std::ofstream file(path);
        file << out.c_str();
        file.close();

    }
    bool AssetMeta::Deserialize(const fs::path& path) {

        CUP_FUNCTION();

        YAML::Node node;
        try { node = YAML::LoadFile(path.string()); } catch (YAML::Exception e) {

            LogError("Failed to load Asset Meta file.\n\tPath: {}\n\tError Message: {}", path.string(), e.what());

            m_path = "";
            m_uuid = UUID::GetInvalid();

            return false;

        }

        m_path = path;
        m_uuid = node["UUID"].as<UUID>();

        return true;

    }

}
