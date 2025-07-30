#include "Model.h"

#include "Core/EditorApp.h"

#include "Projects/Project.h"

#include "Assets/ProjectAssetDatabase.h"

#include <Engine/AssetStorage/AssetMap.h>
#include <Engine/AssetStorage/AssetStorage.h>

#include <Engine/Renderer/Material.h>
#include <Engine/Renderer/Mesh.h>

#include <Engine/Components/MeshRenderer.h>

#include <yaml-cpp/yaml.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Editor {

    void Model::Load(const fs::path& path) {

        CUP_FUNCTION();

        if (m_valid)
            Clear();

        m_path = path;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile((GetProject().GetAssetsPath() / path).string().c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

        if (scene == nullptr) {

            LogError("Could not read model file ({}), error: {}", path, importer.GetErrorString());
            return;

        }

        ProcessMaterials(scene);

        m_meshes.reserve(scene->mNumMeshes);
        ProcessNode(m_rootNode, scene->mRootNode, scene);

        m_valid = true;

    }
    void Model::Clear() {

        CUP_FUNCTION();

        m_rootNode.Reset();

        m_meshes.clear();
        m_materials.clear();
        m_textures.clear();

        m_valid = false;

    }

    void Model::Instantiate(Transform* parent) const {

        CUP_FUNCTION();

        InstantiateNode(m_rootNode, parent);

    }

    void Model::ProcessMaterials(const aiScene* scene) {

        CUP_FUNCTION();

        m_materials.reserve(scene->mNumMaterials);
        for (uint32 i = 0; i < scene->mNumMaterials; i++) {

            const aiMaterial* modelMaterial = scene->mMaterials[i];
            CU_ASSERT(modelMaterial != nullptr, "Material #{} on model {} is nullptr!", i, m_path);

            // Get the name

            std::string name;
            {
                aiString tmp;
                modelMaterial->Get(AI_MATKEY_NAME, tmp);

                name = tmp.C_Str();
            }
            name += ".mat";

            // On first import, the UUID won't be loaded, so we create a new one and load it. It will be stored
            // when ProjectMetadata gets serialized, and loaded on later runs

            UUID uuid = ProjectAssetDatabase::GetAssetFromPath(m_path / name);
            if (!uuid.IsValid()) {

                LogWarn("UUID for material {} of model {} does not exist, creating new one", name, m_path);

                UUID::Generate(uuid);
                ProjectAssetDatabase::AddAsset(m_path / name, uuid);

            }

            MaterialAsset material = AssetStorage::InsertAsset<Material>(uuid);
            m_materials.push_back({ material, name });

            CU_CHECK(modelMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, *reinterpret_cast<aiColor4D*>(&material->albedo)) != AI_SUCCESS, "Could not get albedo color from material {} of model {}", name, m_path);

            // Texture

            aiString tmp;
            if (modelMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &tmp) != AI_SUCCESS) continue;

            fs::path fullPath = tmp.C_Str();
            fs::path texturePath = m_path.parent_path() / fullPath.filename();

            if (!fs::exists(GetProject().GetAssetsPath() / texturePath)) {

                LogError("Missing texture {} for material {} of model {}, model has path {}", texturePath, name, m_path, fullPath);
                continue;

            }

            UUID textureUUID = ProjectAssetDatabase::GetAssetFromPath(texturePath);
            if (!uuid.IsValid()) {

                LogWarn("UUID for texture {} of model material {} of model {} does not exist, creating new one", texturePath.filename(), name, m_path);

                UUID::Generate(uuid);
                ProjectAssetDatabase::AddAsset(texturePath, textureUUID);

            }

            m_textures.push_back({ textureUUID, texturePath.filename().string()});

            material->texture = textureUUID;

        }

    }
    void Model::ProcessNode(Node& node, const aiNode* modelNode, const aiScene* scene) {

        CUP_FUNCTION();

        CU_ASSERT(modelNode != nullptr, "Node is nullptr");

        // Node name

        if (&node == &m_rootNode) {

            node.name = m_path.filename().string();
            node.name.resize(node.name.find_last_of('.'));

        } else if (modelNode->mName.length != 0)
            node.name = modelNode->mName.C_Str();

        // Load mesh (only the first one)

        if (modelNode->mNumMeshes > 1)
            LogWarn("Node ({}) from model {} has more than one mesh, Copper supports single mesh nodes only, loading first mesh", node.name, m_path);
        else if (modelNode->mNumMeshes == 1) {

            const aiMesh* modelMesh = scene->mMeshes[modelNode->mMeshes[0]];
            CU_ASSERT(modelMesh != nullptr, "Could not get mesh from node {}, model: {}", node.name, m_path);

            node.meshIndex = static_cast<uint32>(m_meshes.size());
            node.materialIndex = modelMesh->mMaterialIndex;

            // Mesh name (for asset dtabase purposes)

            std::string meshName;
            if (modelMesh->mName.length > 0)
                meshName = modelMesh->mName.C_Str();
            else
                meshName = std::to_string(m_meshes.size());
            meshName += ".cum";

            // On first import, the UUID won't be loaded, so we create a new one and load it. It will be stored
            // when ProjectMetadata gets serialized, and loaded on later runs

            UUID uuid = ProjectAssetDatabase::GetAssetFromPath(m_path / meshName);
            if (!uuid.IsValid()) {

                LogWarn("UUID for mesh {} of model {} does not exist, creating new one", meshName, m_path);

                UUID::Generate(uuid);
                ProjectAssetDatabase::AddAsset(m_path / meshName, uuid);

            }

            MeshAsset mesh = AssetStorage::InsertAsset<Mesh>(uuid);
            m_meshes.push_back({ mesh, meshName });

            mesh->vertices.clear();
            mesh->normals.clear();
            mesh->colors.clear();
            mesh->uvs.clear();

            for (uint32 i = 0; i < modelMesh->mNumVertices; i++) {

                const aiVector3D& position = modelMesh->mVertices[i];
                const aiVector3D& normal = modelMesh->mNormals[i];

                mesh->vertices.push_back(Vector3(position.x, position.y, position.z));
                mesh->normals.push_back(Vector3(-normal.x, -normal.y, -normal.z));

                // TODO: Maybe there's a better way ?

                if (modelMesh->HasVertexColors(0))
                    mesh->colors.push_back(Color(modelMesh->mColors[0][i].r, modelMesh->mColors[0][i].g, modelMesh->mColors[0][i].b));
                else
                    mesh->colors.push_back(Color::white);

                if (modelMesh->HasTextureCoords(0))
                    mesh->uvs.push_back(Vector2(modelMesh->mTextureCoords[0][i].x, modelMesh->mTextureCoords[0][i].y));
                else
                    mesh->uvs.push_back(Vector2::zero);

            }

            mesh->indices.clear();
            for (uint32 i = 0; i < modelMesh->mNumFaces; i++) {

                const aiFace& face = modelMesh->mFaces[i];
                CU_ASSERT(face.mNumIndices == 3, "face #{} in model {} has more than 3 indices ({})", i, m_path, face.mNumIndices);

                mesh->indices.push_back(face.mIndices[0]);
                mesh->indices.push_back(face.mIndices[1]);
                mesh->indices.push_back(face.mIndices[2]);

            }

        }

        // Process children 

        node.children.resize(modelNode->mNumChildren);
        for (uint32 i = 0; i < modelNode->mNumChildren; i++)
            ProcessNode(node.children[i], modelNode->mChildren[i], scene);

    } 

    void Model::InstantiateNode(const Node& node, Transform* parent) const {

        CUP_FUNCTION();

        // Entity setup (every node is equivalent to an entity)

        Entity entity = CreateEntity(Vector3::zero, Quaternion::identity, Vector3::one, node.name);

        if (parent != nullptr)
            entity->GetTransform()->SetParent(parent);

        // Set the mesh

        if (node.meshIndex != UINT_MAX) {

            CU_ASSERT(m_meshes.size() > node.meshIndex, "Invalid mesh index for node {} of model", node.name, m_path);

            MeshRenderer* renderer = entity->AddComponent<MeshRenderer>();
            CU_ASSERT(renderer != nullptr, "Could not add MeshRenderer to entity {}", *entity);

            renderer->mesh = m_meshes[node.meshIndex].first;

            if (node.materialIndex != UINT_MAX) {

                CU_ASSERT(m_materials.size() > node.materialIndex, "Invalid material index for node {} of model {}", node.name, m_path);
                renderer->material = m_materials[node.materialIndex].first;

            }

        }

        // Process children

        for (const Node& child : node.children)
            InstantiateNode(child, entity->GetTransform());

    }

    void Model::SerializeMetadata(YAML::Emitter& out) const {

        CUP_FUNCTION();

        for (const std::pair<MeshAsset, std::string>& mesh : m_meshes)
            out << YAML::Key << mesh.second << YAML::Value << mesh.first;

        for (const std::pair<MaterialAsset, std::string>& material : m_materials) 
            out << YAML::Key << material.second << YAML::Value << material.first;

    }

}
