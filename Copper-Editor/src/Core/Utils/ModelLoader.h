#pragma once

#include <Copper.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Editor {

	class Model {

	public:
		std::vector<Copper::Mesh> meshes;

		void LoadMesh(const fs::path& path);

	private:

		std::vector<Copper::Vector3> positions;
		std::vector<Copper::Vector3> normals;
		std::vector<uint32_t> indices;

		void ProccessNode(aiNode* node, const aiScene* scene);
		Copper::Mesh ProccessMesh(aiMesh* mesh);

	};

}