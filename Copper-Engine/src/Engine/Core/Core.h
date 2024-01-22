#pragma once

#ifdef CU_CMAKE
    #include <Config.h>
#endif

#include "Engine/Core/Log.h"
#include "Engine/Core/Debug.h"

#define CMATH_PARENT_NAMESPACE Copper
#include <CMath/CMath.h>
#include "Engine/Renderer/Color.h"

namespace Copper {

    namespace fs = std::filesystem;

    using CMath::Vector2;
    using CMath::Vector3;
    using CMath::Vector4;
    using CMath::Matrix3;
    using CMath::Matrix4;

    using CMath::Vector2I;
    using CMath::Vector3I;
    using CMath::Vector4I;
    using CMath::Matrix3I;
    using CMath::Matrix4I;

    using CMath::UVector2I;
    using CMath::UVector3I;
    using CMath::UVector4I;
    using CMath::UMatrix3I;
    using CMath::UMatrix4I;

    using CMath::Quaternion;
    using CMath::QuaternionI;
    using CMath::UQuaternionI;

    typedef char                int8;
    typedef unsigned char       uint8;

    typedef short               int16;
    typedef unsigned short      uint16;

    typedef int                 int32;
    typedef unsigned int        uint32;

    typedef long long           int64;
    typedef unsigned long long  uint64;
    
}