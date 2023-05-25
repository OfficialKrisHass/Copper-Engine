#pragma once

#include "cupch.h"

namespace Copper {

    const std::vector<Vector3>&     GetPlaneVertices();
    const std::vector<Vector3>&     GetPlaneNormals();
    const std::vector<Color>&       GetPlaneColors();
    const std::vector<uint32_t>&    GetPlaneIndices();

    const std::vector<Vector3>&     GetCubeVertices();
    const std::vector<Vector3>&     GetCubeNormals();
    const std::vector<Color>&       GetCubeColors();
    const std::vector<uint32_t>&    GetCubeIndices();
    
}