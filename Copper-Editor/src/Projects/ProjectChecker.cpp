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

			std::ifstream dllSrc(ExecutableFolder() + "/assets/ScriptingAPI/Copper-ScriptingAPI.dll", std::ios::binary);
			std::ofstream dllDst;

			dllDst.open(project.GetPath() / "Binaries/Copper-ScriptingAPI.dll", std::ios::out | std::ios::binary);
			dllDst << dllSrc.rdbuf();
            dllDst.flush();
			dllDst.close();

		}
		if (GET_FLAG(issueFlags, MissingProjectDLL))
            project.BuildScripts();

	}

}
