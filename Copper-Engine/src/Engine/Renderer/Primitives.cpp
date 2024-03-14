#include "cupch.h"
#include "Primitives.h"

#include "Engine/Renderer/PrimitivesData.h"

#include "Engine/AssetStorage/AssetStorage.h"

namespace Copper {

	static const UUID planeUUID = UUID::fromStrFactory("3fc27a27-5dc3-4941-bd00-f540d35f4eff");
	static const UUID cubeUUID = UUID::fromStrFactory("25a5d2e1-6959-4130-aed9-5195f0fe4789");

	MeshAsset planeMesh;
	MeshAsset cubeMesh;

	void InitializePrimitives() {

		// Plane Mesh

		planeMesh = AssetStorage::InsertAsset<Mesh>(planeUUID);

		planeMesh->vertices = planeVertices;
		planeMesh->indices = planeIndices;
		planeMesh->normals = planeNormals;
		planeMesh->colors = planeColors;
		planeMesh->uvs = planeUvs;

		// Cube Mesh

		cubeMesh = AssetStorage::InsertAsset<Mesh>(cubeUUID);

		cubeMesh->vertices = cubeVertices;
		cubeMesh->indices = cubeIndices;
		cubeMesh->normals = cubeNormals;
		cubeMesh->colors = cubeColors;
		cubeMesh->uvs = cubeUvs;

	}

	const MeshAsset& PlaneMesh() { return planeMesh; }
	const MeshAsset& CubeMesh() { return cubeMesh; }

}