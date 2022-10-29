#include "ModelLoader.h"

namespace Editor {

	using namespace Copper;

	void Model::LoadMesh(std::filesystem::path path) {

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path.make_preferred().string().c_str(), (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices));

		if (!scene) { LogError("Error Parsing FBX model file!\nPath: {0}\nError:\n\n{1}", path, importer.GetErrorString()); return; }

		uint32_t numVertices = 0;
		uint32_t numIndices = 0;

		for (int i = 0; i < meshes.size(); i++) {

			numVertices += scene->mMeshes[i]->mNumVertices;
			numIndices  += scene->mMeshes[i]->mNumFaces * 3;

		}

		positions.reserve(numVertices);
		normals.reserve(numVertices);
		indices.reserve(numIndices);

		ProccessNode(scene->mRootNode, scene);

		return;

	}

	void Model::ProccessNode(aiNode* node, const aiScene* scene) {

		for (int i = 0; i < node->mNumMeshes; i++) {

			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProccessMesh(mesh));

		}

		for (int i = 0; i < node->mNumChildren; i++) {

			ProccessNode(node->mChildren[i], scene);

		}

	}

	Copper::Mesh Model::ProccessMesh(aiMesh* mesh) {

		Mesh ret;

		for (int i = 0; i < mesh->mNumVertices; i++) {

			const aiVector3D& position = mesh->mVertices[i];
			const aiVector3D& normal = mesh->mNormals[i];

			ret.vertices.push_back(Vector3(position.x, position.y, position.z));
			ret.normals.push_back(-Vector3(normal.x, normal.y, normal.z));

		}

		for (int i = 0; i < mesh->mNumFaces; i++) {

			aiFace& face = mesh->mFaces[i];

			ret.indices.push_back(face.mIndices[0]);
			ret.indices.push_back(face.mIndices[1]);
			ret.indices.push_back(face.mIndices[2]);

		}

		std::vector<Color> colors;
		for (int i = 0; i < mesh->mNumVertices; i++) { colors.push_back(Color::White()); }
		ret.colors = colors;

		return ret;

	}

}