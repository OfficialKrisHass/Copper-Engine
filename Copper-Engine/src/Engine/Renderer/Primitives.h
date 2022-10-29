#pragma once

namespace Copper {

#pragma region Plane
    std::vector<Vector3> planeVertices {

        Vector3(-0.5f, 0.0f,  0.5f),
        Vector3( 0.5f, 0.0f,  0.5f),
        Vector3( 0.5f, 0.0f, -0.5f),
        Vector3(-0.5f, 0.0f, -0.5f),

    };
    std::vector<Vector3> planeNormals {

        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),

    };
    std::vector<Color> planeColors {

        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),

    };
    std::vector<uint32_t> planeIndices {

        0, 1, 2,
        2, 3, 0
        
    };
#pragma endregion
#pragma region Cube
    std::vector<Vector3> cubeVertices {
        //Front
        Vector3(-0.5f, -0.5f,  0.5f),
        Vector3( 0.5f, -0.5f,  0.5f),
        Vector3( 0.5f,  0.5f,  0.5f),
        Vector3(-0.5f,  0.5f,  0.5f),
        //Back
        Vector3( 0.5f, -0.5f, -0.5f),
        Vector3(-0.5f, -0.5f, -0.5f),
        Vector3(-0.5f,  0.5f, -0.5f),
        Vector3( 0.5f,  0.5f, -0.5f),
        //Right
        Vector3( 0.5f, -0.5f,  0.5f),
        Vector3( 0.5f, -0.5f, -0.5f),
        Vector3( 0.5f,  0.5f, -0.5f),
        Vector3( 0.5f,  0.5f,  0.5f),
        //Left
        Vector3(-0.5f, -0.5f, -0.5f),
        Vector3(-0.5f, -0.5f,  0.5f),
        Vector3(-0.5f,  0.5f,  0.5f),
        Vector3(-0.5f,  0.5f, -0.5f),
        //Up
        Vector3(-0.5f,  0.5f,  0.5f),
        Vector3( 0.5f,  0.5f,  0.5f),
        Vector3( 0.5f,  0.5f, -0.5f),
        Vector3(-0.5f,  0.5f, -0.5f),
        //Down
        Vector3(-0.5f, -0.5f, -0.5f),
        Vector3( 0.5f, -0.5f, -0.5f),
        Vector3( 0.5f, -0.5f,  0.5f),
        Vector3(-0.5f, -0.5f,  0.5f),

    };
    std::vector<Vector3> cubeNormals {
        //Front
        Vector3( 0.0f,  0.0f, -1.0f),
        Vector3( 0.0f,  0.0f, -1.0f),
        Vector3( 0.0f,  0.0f, -1.0f),
        Vector3( 0.0f,  0.0f, -1.0f),
        //Back
        Vector3( 0.0f,  0.0f,  1.0f),
        Vector3( 0.0f,  0.0f,  1.0f),
        Vector3( 0.0f,  0.0f,  1.0f),
        Vector3( 0.0f,  0.0f,  1.0f),
        //Right
        Vector3(-1.0f,  0.0f,  0.0f),
        Vector3(-1.0f,  0.0f,  0.0f),
        Vector3(-1.0f,  0.0f,  0.0f),
        Vector3(-1.0f,  0.0f,  0.0f),
        //Left
        Vector3( 1.0f,  0.0f,  0.0f),
        Vector3( 1.0f,  0.0f,  0.0f),
        Vector3( 1.0f,  0.0f,  0.0f),
        Vector3( 1.0f,  0.0f,  0.0f),
        //Up
        Vector3( 0.0f, -1.0f,  0.0f),
        Vector3( 0.0f, -1.0f,  0.0f),
        Vector3( 0.0f, -1.0f,  0.0f),
        Vector3( 0.0f, -1.0f,  0.0f),
        //Down
        Vector3( 0.0f,  1.0f,  0.0f),
        Vector3( 0.0f,  1.0f,  0.0f),
        Vector3( 0.0f,  1.0f,  0.0f),
        Vector3( 0.0f,  1.0f,  0.0f),

    };
    std::vector<Color> cubeColors {
        //Front
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        //Back
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        //Right
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        //Left
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        //Up
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        //Down
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),
        Color(1.0f, 1.0f, 1.0f),

    };
    std::vector<uint32_t> cubeIndices {

        //Front
        0, 1, 2,
        2, 3, 0,

        //Back
        4, 5, 6,
        6, 7, 4,

        //Right
        8, 9, 10,
        10, 11, 8,

        //Left
        12, 13, 14,
        14, 15, 12,

        //Up
        16, 17, 18,
        18, 19, 16,

        //Down
        20, 21, 22,
        22, 23, 20
        
    };
#pragma endregion 
    
}