#pragma once

#include <Copper.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Editor {

	class Model {

	public:
		std::vector<Copper::Mesh> meshes;

		void LoadMesh(const Copper::fs::path& path);

	private:
		std::vector<Copper::Vector3> m_positions;
		std::vector<Copper::Vector3> m_normals;
		std::vector<Copper::uint32>  m_indices;

		void ProccessNode(aiNode* node, const aiScene* scene);
		Copper::Mesh ProccessMesh(aiMesh* mesh);

	};

}