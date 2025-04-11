#include "Model.h"

#include "Core/EditorApp.h"

#include "Projects/Project.h"

#include "Assets/ProjectAssetDatabase.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Editor {

    Model::Model(const fs::path& path) {

        CUP_FUNCTION();

        m_path = path;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile((GetProject().GetAssetsPath() / path).string().c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

        if (scene == nullptr) {

            LogError("Could not read model file ({}), error: {}", path, importer.GetErrorString());
            return;

        }

        m_meshes.reserve(scene->mNumMeshes);
        ProcessNode(m_rootNode, scene->mRootNode, scene);

    }

    void Model::Instantiate(Transform* parent) const {

        CUP_FUNCTION();

        InstantiateNode(m_rootNode, parent);

    }

    void Model::ProcessNode(Node& node, const aiNode* modelNode, const aiScene* scene) {

        CUP_FUNCTION();

        CU_ASSERT(modelNode != nullptr, "Node is nullptr");

        if (&node == &m_rootNode) {

            node.name = m_path.filename();
            node.name.resize(node.name.find_last_of('.'));

        } else if (modelNode->mName.length != 0)
            node.name = modelNode->mName.C_Str();

        if (modelNode->mNumMeshes > 1)
            LogWarn("Node ({}) from model {} has more than one mesh, Copper supports single mesh nodes only, loading first mesh", node.name, m_path);
        else if (modelNode->mNumMeshes == 1) {

            const aiMesh* modelMesh = scene->mMeshes[modelNode->mMeshes[0]];
            CU_ASSERT(modelMesh != nullptr, "Could not get mesh from node {}, model: {}", node.name, m_path);

            node.index = m_meshes.size();

            std::string meshName = "_";
            if (modelMesh->mName.length > 0)
                meshName += modelMesh->mName.C_Str();
            else
                meshName += std::to_string(m_meshes.size());
            meshName += ".cum";

            UUID uuid = ProjectAssetDatabase::GetAssetFromPath(m_path / meshName);
            if (uuid == UUID::GetInvalid()) {

                LogWarn("UUID for mesh {} of model {} does not exist, creating new one", meshName, m_path);
                UUID::Generate(uuid);

            }

            MeshAsset mesh = AssetStorage::InsertAsset<Mesh>(uuid);
            m_meshes.push_back({ mesh, meshName });

            mesh->vertices.reserve(modelMesh->mNumVertices);
            mesh->normals.reserve(modelMesh->mNumVertices);
            mesh->colors.reserve(modelMesh->mNumVertices);
            mesh->uvs.reserve(modelMesh->mNumVertices);

            for (uint32 i = 0; i < modelMesh->mNumVertices; i++) {

                const aiVector3D& position = modelMesh->mVertices[i];
                const aiVector3D& normal = modelMesh->mNormals[i];

                mesh->vertices.push_back(Vector3(position.x, position.y, position.z));
                mesh->normals.push_back(Vector3(-normal.x, -normal.y, -normal.z));

                if (modelMesh->HasVertexColors(0))
                    mesh->colors.push_back(Color(modelMesh->mColors[0][i].r, modelMesh->mColors[0][i].g, modelMesh->mColors[0][i].b));
                else
                    mesh->colors.push_back(Color::white);

                if (modelMesh->HasTextureCoords(0))
                    mesh->uvs.push_back(Vector2(modelMesh->mTextureCoords[0][i].x, modelMesh->mTextureCoords[0][i].y));
                else
                    mesh->uvs.push_back(Vector2::zero);

            }

            mesh->indices.reserve(modelMesh->mNumFaces * 3);
            for (uint32 i = 0; i < modelMesh->mNumFaces; i++) {

                const aiFace& face = modelMesh->mFaces[i];
                CU_ASSERT(face.mNumIndices == 3, "face #{} in model {} has more than 3 indices ({})", i, m_path, face.mNumIndices);

                mesh->indices.push_back(face.mIndices[0]);
                mesh->indices.push_back(face.mIndices[1]);
                mesh->indices.push_back(face.mIndices[2]);

            }

        }

        node.children.resize(modelNode->mNumChildren);
        for (uint32 i = 0; i < modelNode->mNumChildren; i++)
            ProcessNode(node.children[i], modelNode->mChildren[i], scene);

    } 
    void Model::InstantiateNode(const Node& node, Transform* parent) const {

        CUP_FUNCTION();

        Entity entity = CreateEntity(Vector3::zero, Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vector3::one, node.name);

        if (parent != nullptr)
            entity->GetTransform()->SetParent(parent);

        if (node.index != UINT_MAX) {

            CU_ASSERT(m_meshes.size() > node.index, "Invalid mesh index for node {}", node.name);

            MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();
            CU_ASSERT(renderer != nullptr, "Could not add MeshRenderer to entity {}", *entity);

            renderer->mesh = m_meshes[node.index].first;

        }

        for (const Node& child : node.children)
            InstantiateNode(child, entity->GetTransform());

    }

    void Model::SerializeMetadata(YAML::Emitter& out) const {

        CUP_FUNCTION();

        for (const std::pair<MeshAsset, std::string>& mesh : m_meshes)
            out << YAML::Key << mesh.second << YAML::Value << mesh.first;

    }

}

namespace Copper::AssetStorage {

    AssetMap<Editor::Model> modelMap;

    template<> AssetMap<Editor::Model>& GetAssetMap<Editor::Model>() { return modelMap; }

}
