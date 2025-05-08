#include "ProjectMetadata.h"

#include "Core/EditorApp.h"

#include "Projects/Project.h"

#include "Assets/ProjectAssetDatabase.h"
#include "Assets/Model.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Editor::ProjectMetadata {

    using namespace Copper;

    void SerializeAsset(YAML::Emitter& out, const std::pair<fs::path, UUID>& assetMeta);
    void DeserializeAsset(const YAML::const_iterator& it, std::unordered_map<fs::path, UUID>& assetMetas);

    void Serialize(const std::unordered_map<fs::path, UUID>& assetMetas) {

        CUP_FUNCTION();

        YAML::Emitter out;
        out << YAML::BeginMap; // Main

        out << YAML::Key << "Assets" << YAML::Value << YAML::BeginMap; // Assets
        
        for (const auto& it : assetMetas)
            SerializeAsset(out, it);

        out << YAML::EndMap; // Assets

        out << YAML::EndMap; // Main

        std::ofstream file;
        file.open(GetProject().GetPath() / "ProjectMetadata.cu");
        file << out.c_str();
        file.close();

    }
    void Deserialize(std::unordered_map<fs::path, UUID>& assetMetas) {

        CUP_FUNCTION();

        YAML::Node node;
        try { node = YAML::LoadFile((GetProject().GetPath() / "ProjectMetadata.cu").string()); }
        catch (YAML::Exception e) {

            LogError("Could not load project metadata (should be at {}). Error: {}.", GetProject().GetPath() / "ProjectMetadata.cu", e.what());
            return;

        }

        YAML::Node assets = node["Assets"];
        for (YAML::const_iterator it = assets.begin(); it != assets.end(); ++it)
            DeserializeAsset(it, assetMetas);

    }

    void SerializeAsset(YAML::Emitter& out, const std::pair<fs::path, UUID>& assetMeta) {

        CUP_FUNCTION();

        const fs::path& path = assetMeta.first;
        if (!fs::exists(GetProject().GetAssetsPath() / path)) return;

        out << YAML::Key << path << YAML::Value << YAML::BeginMap; // Asset

        out << YAML::Key << "UUID" << YAML::Value << assetMeta.second;
        if (path.extension() == ".fbx") {

            out << YAML::Key << "Sub Assets" << YAML::Value << YAML::BeginMap;

            const ModelAsset model = static_cast<const ModelAsset>(assetMeta.second);
            model->SerializeMetadata(out);

            out << YAML::EndMap;

        }

        out << YAML::EndMap; // Asset

    }
    void DeserializeAsset(const YAML::const_iterator& it, std::unordered_map<fs::path, UUID>& assetMetas) {

        CUP_FUNCTION();

        const fs::path path = it->first.as<fs::path>();
        const YAML::Node& metadata = it->second;

        if (!fs::exists(GetProject().GetAssetsPath() / path)) return;

        assetMetas[path] = metadata["UUID"].as<UUID>(); 

        const YAML::Node& subAssets = metadata["Sub Assets"];
        if (!subAssets) return;

        for (YAML::const_iterator sub = subAssets.begin(); sub != subAssets.end(); ++sub)
            ProjectAssetDatabase::AddAsset(sub->second.as<UUID>(), path / sub->first.as<std::string>());

    }

}
