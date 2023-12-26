#pragma once

#ifdef CU_CMAKE
    #include <Config.h>
#endif

#include "Engine/Core/Log.h"

#define CMATH_PARENT_NAMESPACE Copper
#include <CMath/CMath.h>

#include "Engine/Renderer/Color.h"

#include <memory>

#define CHECK(x, ...) { if(!(x)) { LogError(__VA_ARGS__); } }

#ifdef CU_DEBUG
    #define CU_ASSERT(x, ...) CHECK(x, __VA_ARGS__);
#else
    #define CU_ASSERT(x, ...)
#endif

#ifdef CU_WINDOWS
namespace fs = std::filesystem;
#elif CU_LINUX
namespace fs = std::filesystem;
#endif

namespace Copper {

    template<typename T> using Unique = std::unique_ptr<T>;
    template<typename T, typename ... Args> constexpr  Unique<T> CreateUnique(Args&& ... args) {

        return std::make_unique<T>(std::forward<Args>(args)...);
        
    }

    template<typename T> using Shared = std::shared_ptr<T>;
    template<typename T, typename ... Args> constexpr  Shared<T> CreateShared(Args&& ... args) {

        return std::make_shared<T>(std::forward<Args>(args)...);
        
    }

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
    
}