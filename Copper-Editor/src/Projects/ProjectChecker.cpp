#include "ProjectChecker.h"

#include "Projects/Project.h"

#define CHECK_FOLDER(folderName, folderPath, x) if (!std::filesystem::exists(path + folderPath)) { LogError("Project '{}' is missing the {} folder", project.name, folderName); ret |= BIT(x); }
#define CHECK_FILE(fileName, filePath, x) if (!std::filesystem::exists(path + filePath)) { LogError("Project '{}' is missing the {} file", project.name, fileName); ret |= BIT(x); }

#define FLAG(x, flag) x & (uint16_t) flag

namespace Editor::ProjectChecker {

	uint16_t CheckProject(const Project& project) {

		const std::string path = project.path.String() + "/";
		uint16_t ret = 0;

		// Base folders

		CHECK_FOLDER("Assets", "Assets", 0);
		CHECK_FOLDER("Binaries", "Binaries", 1);

		// Binaries

		CHECK_FILE("Project assembly (" + project.name + ".dll)", "Binaries/" + project.name + ".dll", 2);
		CHECK_FILE("Scripting API assembly (Copper-ScriptingAPI.dll)", "Binaries/Copper-ScriptingAPI.dll", 3);

		// Project files

		CHECK_FILE("Project file (Project.cu)", "Project.cu", 4);

	#ifdef CU_WINDOWS

		CHECK_FILE("Visual Studio project (" + project.name + ".csproj)", project.name + ".csproj", 5);
		CHECK_FILE("Visual Studio solution (" + project.name + ".sln)", project.name + ".sln", 6);

	#elif CU_LINUX

		CHECK_FILE("Makefile", "Makefile", 5);
		CHECK_FILE("Premake (premake5.lua)", "premake5.lua", 6);

	#endif

		return ret;
		
	}
	void FixProject(Project& project, const uint16_t issueFlags) {

		const std::string path = project.path.String() + "/";

		if (FLAG(issueFlags, MissingAssets)) std::filesystem::create_directories(path + "Assets");
		if (FLAG(issueFlags, MissingBinaries)) std::filesystem::create_directories(path + "Binaries");

		if (FLAG(issueFlags, MissingProjectFile)) {

			project.name = ((std::filesystem::path) path).parent_path().filename().string();
			project.RegenerateProjectFile();
			
		}

	#ifdef CU_WINDOWS
		if (FLAG(issueFlags, MissingSolution) || FLAG(issueFlags, MissingCSProj)) project.RegenerateIDEFiles();
	#elif CU_LINUX
		if (FLAG(issueFlags, MissingPremake)) project.RegenerateIDEFiles();
		if (FLAG(issueFlags, MissingMakefile)) project.RunPremake();
	#endif

		if (FLAG(issueFlags, MissingProjectDLL)) project.BuildSolution();
		if (FLAG(issueFlags, MissingScriptingDLL)) {

			std::ifstream dllSrc("assets/ScriptAPI/Copper-ScriptingAPI.dll", std::ios::binary);
			std::fstream dllDst;

			dllDst.open(path + "Binaries/Copper-ScriptingAPI.dll", std::ios::out | std::ios::binary);
			dllDst << dllSrc.rdbuf();
			dllDst.close();

		}

	}

}