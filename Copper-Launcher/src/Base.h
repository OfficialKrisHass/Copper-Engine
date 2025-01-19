#pragma once

#include <cstdio>
#include <string>

#include <filesystem>

namespace Launcher {

    namespace fs = std::filesystem;

    inline constexpr float WindowPadding = 10.0f;
    inline constexpr const char* MainFontPath = "assets/fonts/open-sans.regular.ttf";

#ifdef CU_LINUX
    inline const std::string BaseProjectDir = std::string("/home/") + cuserid(nullptr);
#elif CU_WINDOWS
    inline const std::string BaseProjectDir = "C:\\";
#endif

    typedef char                int8;
    typedef unsigned char       uint8;

    typedef short               int16;
    typedef unsigned short      uint16;

    typedef int                 int32;
    typedef unsigned int        uint32;

    typedef long long           int64;
    typedef unsigned long long  uint64;

    const fs::path& ExecutableFolder();

    void LaunchEditor(const std::string& projectPath);

    void RemoveEntry(const class ProjectEntry& entry);

}
