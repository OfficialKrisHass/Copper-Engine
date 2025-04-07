#include "ModelLoader.h"

#include "Engine/AssetStorage/AssetStorage.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Editor::ModelLoader {

    void Load(const fs::path &path, const UUID& uuid) {

        CUP_FUNCTION();

        CU_ASSERT(path.extension() == ".fbx", "Attempted to load an unsupported file {}", path); 

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

        if (scene == nullptr) {

            LogError("Could not import model file ({}), error: {}", path, importer.GetErrorString());
            return;

        }

        aiNode* node = scene->mRootNode->mChildren[0]; // Hack, for some reason blender fbx models have the mesh in the child of the rootNode
        if (node->mNumChildren != 0 || node->mNumMeshes > 1)
            LogWarn("Model {} contains more than 1 mesh, Copper currently only supports loading the root mesh", path);

        aiMesh* modelMesh = scene->mMeshes[node->mMeshes[0]];
        MeshAsset mesh = AssetStorage::InsertAsset<Mesh>(uuid);

        mesh->vertices.reserve(modelMesh->mNumVertices);
        mesh->vertices.reserve(modelMesh->mNumFaces * 3);

        for (uint32 i = 0; i < modelMesh->mNumVertices; i++) {

            const aiVector3D& pos = modelMesh->mVertices[i];
            const aiVector3D& normal = modelMesh->mNormals[i];

            mesh->vertices.push_back(Vector3(pos.x, pos.y, pos.z));
            mesh->normals.push_back(Vector3(-normal.x, -normal.y, -normal.z));
            mesh->colors.push_back(Color::white);

            if (modelMesh->HasTextureCoords(0))
                mesh->uvs.push_back(Vector2(modelMesh->mTextureCoords[0][i].x, modelMesh->mTextureCoords[0][i].y));

        }

        for (uint32 i = 0; i < modelMesh->mNumFaces; i++) {

            const aiFace& face = modelMesh->mFaces[i];
            CU_ASSERT(face.mNumIndices == 3, "face #{} in model {} has more than 3 faces ({})", i, path, face.mNumIndices);

            mesh->indices.push_back(face.mIndices[0]);
            mesh->indices.push_back(face.mIndices[1]);
            mesh->indices.push_back(face.mIndices[2]);

        }

    }

}
