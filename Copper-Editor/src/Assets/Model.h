#pragma once

#include <Copper.h>

struct aiScene;
struct aiNode;

namespace Editor {

    using namespace Copper;

    class Model {

    public:
        Model() = default;
        Model(const fs::path& path);

        void Instantiate(Transform* parent) const;

        void SerializeMetadata(YAML::Emitter& out) const;

    private:
        struct Node {

            std::string name = "Node";

            uint32 index = UINT_MAX;
            std::vector<Node> children;

        };

        fs::path m_path;

        std::vector<std::pair<MeshAsset, std::string>> m_meshes;
        Node m_rootNode;

        void ProcessNode(Node& node, const aiNode* modelNode, const aiScene* scene);
        void InstantiateNode(const Node& node, Transform* parent) const;

    };

    typedef Copper::AssetPtr<Model> ModelAsset;

}
