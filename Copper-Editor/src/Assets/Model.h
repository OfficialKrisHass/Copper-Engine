#pragma once

#include <Copper.h>

#include <Engine/Renderer/Texture.h>
#include <Engine/Renderer/Material.h>
#include <Engine/Renderer/Mesh.h>

struct aiScene;
struct aiNode;

namespace Editor {

    using namespace Copper;

    class Model {

    public:
        Model() = default;
        Model(const fs::path& path) {

            CUP_FUNCTION();

            Load(path);

        }

        void Load(const fs::path& path);
        void Clear();

        void Instantiate(Transform* parent) const;

        void SerializeMetadata(YAML::Emitter& out) const;

    private:
        struct Node {

            // TODO: Implement per node transforms

            std::string name = "Node";

            uint32 meshIndex = UINT_MAX;
            uint32 materialIndex = UINT_MAX;

            std::vector<Node> children;

            void Reset() {

                CUP_FUNCTION();

                name = "Node";
                meshIndex = materialIndex = UINT_MAX;

                children.clear();

            }

        };

        fs::path m_path;
        bool m_valid = false;

        std::vector<std::pair<MeshAsset, std::string>> m_meshes;
        std::vector<std::pair<MaterialAsset, std::string>> m_materials;
        std::vector<std::pair<TextureAsset, std::string>> m_textures;

        Node m_rootNode;

        void ProcessMaterials(const aiScene* scene);
        void ProcessNode(Node& node, const aiNode* modelNode, const aiScene* scene);

        void InstantiateNode(const Node& node, Transform* parent) const;

    };

}

REGISTER_ASSET_TYPE(Editor, Model);
