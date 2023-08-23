#pragma once

#include <Config.h>

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

namespace Copper {

    using Vector2 = CMath::Vector2;
    using Vector3 = CMath::Vector3;
    using Vector4 = CMath::Vector4;
    using Matrix3 = CMath::Matrix3;
    using Matrix4 = CMath::Matrix4;

    using Vector2I = CMath::Vector2I;
    using Vector3I = CMath::Vector3I;
    using Vector4I = CMath::Vector4I;
    using Matrix3I = CMath::Matrix3I;
    using Matrix4I = CMath::Matrix4I;

    using UVector2I = CMath::UVector2I;
    using UVector3I = CMath::UVector3I;
    using UVector4I = CMath::UVector4I;
    using UMatrix3I = CMath::UMatrix3I;
    using UMatrix4I = CMath::UMatrix4I;

    using Quaternion = CMath::Quaternion;
    using QuaternionI = CMath::QuaternionI;
    using UQuaternionI = CMath::UQuaternionI;

    template<typename T> using Unique = std::unique_ptr<T>;
    template<typename T, typename ... Args> constexpr  Unique<T> CreateUnique(Args&& ... args) {

        return std::make_unique<T>(std::forward<Args>(args)...);
        
    }

    template<typename T> using Shared = std::shared_ptr<T>;
    template<typename T, typename ... Args> constexpr  Shared<T> CreateShared(Args&& ... args) {

        return std::make_shared<T>(std::forward<Args>(args)...);
        
    }
    
}