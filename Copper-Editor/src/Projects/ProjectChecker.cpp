#include "ProjectChecker.h"

#include "Projects/Project.h"

#include <filesystem>
#include <fstream>

#define CHECK_FOLDER(folderName, x) if (!std::filesystem::exists(path / folderName)) {\
                                        LogError("Project '{}' is missing the {} folder", project.GetName(), folderName);\
                                        ret |= FLAG(x); }
#define CHECK_FILE(fileName, filePath, x) if (!std::filesystem::exists(path / filePath)) {\
                                            LogError("Project '{}' is missing the {} ({}) file", project.GetName(), fileName, filePath);\
                                            ret |= FLAG(x); }

#define GET_FLAG(x, flag) x & (uint16) flag

namespace Editor::ProjectChecker {

    using namespace Copper;

    uint16 CheckProject(const Project& project) {

        CUP_FUNCTION();

        const fs::path& path = project.GetPath();
        uint16 ret = 0;

        // Base folders

        CHECK_FOLDER("Assets", 0);
        CHECK_FOLDER("Binaries", 1);

        // Binaries

        CHECK_FILE("Project assembly", ("Binaries/" + project.GetName() + ".dll"), 2);
        CHECK_FILE("Scripting API assembly", "Binaries/Copper-ScriptingAPI.dll", 3);

        // Project files

        CHECK_FILE("Project file", "Project.cu", 4);

#ifdef CU_WINDOWS

        CHECK_FILE("Visual Studio project", (project.GetName() + ".csproj"), 5);
        CHECK_FILE("Visual Studio solution", (project.GetName() + ".sln"), 6);

#elif CU_LINUX

        CHECK_FILE("Makefile", "Makefile", 5);
        CHECK_FILE("Premake", "premake5.lua", 6);

#endif

        return ret;

    }
    void FixProject(Project& project, const uint16 issueFlags) {

        CUP_FUNCTION();

        if (GET_FLAG(issueFlags, MissingAssets))
            std::filesystem::create_directories(project.GetAssetsPath());
        if (GET_FLAG(issueFlags, MissingBinaries))
            std::filesystem::create_directories(project.GetPath() / "Binaries");

        if (GET_FLAG(issueFlags, MissingProjectFile)) {

            project.SetName(project.GetPath().parent_path().filename().string());
            project.RegenerateProjectFiles();

        }

#ifdef CU_WINDOWS
        if (GET_FLAG(issueFlags, MissingSolution) || GET_FLAG(issueFlags, MissingCSProj)) project.RegenerateBuildFiles();
#elif CU_LINUX
        if (GET_FLAG(issueFlags, MissingPremake)) project.RegenerateBuildFiles();
        if (GET_FLAG(issueFlags, MissingMakefile)) project.RunPremake();
#endif
        if (GET_FLAG(issueFlags, MissingScriptingDLL)) {

            std::ifstream dllSrc(ExecutableFolder() / "assets/Copper-ScriptingAPI.dll", std::ios::binary);
            std::ofstream dllDst;

            dllDst.open(project.GetPath() / "Binaries/Copper-ScriptingAPI.dll", std::ios::out | std::ios::binary);
            dllDst << dllSrc.rdbuf();
            dllDst.flush();
            dllDst.close();

        }
        if (GET_FLAG(issueFlags, MissingProjectDLL))
            project.BuildScripts();

        Log("\tIssues fixed.");

    }

    void PrintIssues(Project& project, uint16 issueFlags, std::string &out) {

        CUP_FUNCTION();

        if (GET_FLAG(issueFlags, MissingAssets))
            out += "Missing Assets folder\n";
        if (GET_FLAG(issueFlags, MissingBinaries))
            out += "Missing Binaries folder\n";

        if (GET_FLAG(issueFlags, MissingProjectFile))
            out += "Missing project file (Project.cu)\n";

#ifdef CU_WINDOWS
        if (GET_FLAG(issueFlags, MissingSolution))
            out += "Missing Visual Studio solution (" + project.GetName() + ".sln)\n";
        if (GET_FLAG(issueFlags, MissingCSProj))
            out += "Missing Visual Studio project file (" + project.GetName() + ".csproj)\n";
#elif CU_LINUX
        if (GET_FLAG(issueFlags, MissingPremake))
            out += "Missing premake file (premake5.lua)\n";
        if (GET_FLAG(issueFlags, MissingMakefile))
            out += "Missing makefile (Makefile)\n";
#endif
        if (GET_FLAG(issueFlags, MissingScriptingDLL))
            out += "Missing ScriptingAPI dll file (Binaries/Copper-ScriptingAPI.dll)\n";

        if (GET_FLAG(issueFlags, MissingProjectDLL))
            out += "Missing project dll file (Binaries/" + project.GetName() + ".dll)\n";

    }

}
