#pragma once

#ifdef CU_CMAKE
#include <Config.h>
#endif

#include "Engine/Core/Log.h"

#include "Engine/Debug/Assert.h"
#include "Engine/Debug/Profiler.h"

#define CMATH_PARENT_NAMESPACE Copper
#include <CMath/CMath.h>
#include "Engine/Renderer/Color.h"

#include <filesystem> // Replace with our filesystem implementation

#define FLAG(bit) (1 << bit)

#ifdef CU_LOG_STATUS
#define LogStatus(...) LogDebug(__VA_ARGS__)
#else
#define LogStatus(...) 
#endif

#ifdef CU_EDITOR
namespace Editor {

    extern bool IsRuntimeRunning();

}

#define IN_RUNTIME(action) if (Editor::IsRuntimeRunning()) { action; }
#define IN_NOT_RUNTIME(action) if (!Editor::IsRuntimeRunning()) { action; }
#define IN_RUNTIME_BEGIN if (Editor::IsRuntimeRunning()) {
#define IN_RUNTIME_END }
#else
#define IN_RUNTIME(action) action;
#define IN_NOT_RUNTIME(action)
#define IN_RUNTIME_BEGIN
#define IN_RUNTIME_END
#endif

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

    const fs::path& ExecutableDirectory(); // The directory where the current executable is located.
    const fs::path& ResourceDirectory(); // The directory where the application resources (assets, libs, etc) are stored. On windows this is the same as ExecutableDirectory().
    const fs::path& DataDirectory(); // The data directory of the application, this is where application data should be stored. On Windows this returns %appdata%, and on linux it returns $XDG_DATA_HOME.

}

template<> struct fmt::formatter<Copper::fs::path> : fmt::formatter<std::string> {

    auto format(const Copper::fs::path& path, format_context& ctx) const -> decltype(ctx.out()) {

        return fmt::format_to(ctx.out(), "{}", path.string());

    }

};

template<typename T> struct fmt::formatter<Copper::CMath::vec<2, T>> : fmt::formatter<std::string> {

    auto format(const Copper::CMath::vec<2, T>& vec, format_context& ctx) const -> decltype(ctx.out()) {

        return fmt::format_to(ctx.out(), "X: {{{}}} Y: {{{}}}", vec.x, vec.y);

    }

};
template<typename T> struct fmt::formatter<Copper::CMath::vec<3, T>> : fmt::formatter<std::string> {

    auto format(const Copper::CMath::vec<3, T>& vec, format_context& ctx) const -> decltype(ctx.out()) {

        return fmt::format_to(ctx.out(), "X: {{{}}} Y: {{{}}} Z: {{{}}}", vec.x, vec.y, vec.z);

    }

};
template<typename T> struct fmt::formatter<Copper::CMath::vec<4, T>> : fmt::formatter<std::string> {

    auto format(const Copper::CMath::vec<4, T>& vec, format_context& ctx) const -> decltype(ctx.out()) {

        return fmt::format_to(ctx.out(), "X: {{{}}} Y: {{{}}} Z: {{{}}} W: {{{}}}", vec.x, vec.y, vec.z, vec.w);

    }

};
