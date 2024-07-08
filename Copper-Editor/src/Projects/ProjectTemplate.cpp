#include "ProjectTemplate.h"

#include <fstream>

using std::filesystem::create_directories;

namespace Editor {

    using namespace Copper;

	void CreateFileAndReplace(const fs::path& original, const fs::path& out, const std::string& what, const std::string& replace);

	void CopyFileTo(const fs::path& original, const fs::path& where, bool binary = false);

	void CreateTemplateFromProject(const Project& project) {

        CUP_FUNCTION();

		const fs::path path = "assets/Templates/" + project.GetName();

		create_directories(path.string() + "/Assets/Scenes");

		CreateFileAndReplace(project.GetPath() / "Project.cu", path / "Project.cu.cut", project.GetName(), ":{ProjectName}");

		CopyFileTo(project.GetAssetsPath() / project.GetLastOpenedScenePath(), path / "Assets" / project.GetLastOpenedScenePath());
		CopyFileTo(project.GetAssetsPath() / (project.GetLastOpenedScenePath().string() + ".cum"), path / "Assets" / (project.GetLastOpenedScenePath().string() + ".cum"));

	}
	void CreateProjectFromTemplate(const fs::path& templatePath, Project& project) {

        CUP_FUNCTION();

		create_directories(project.GetPath().string() + "/Assets/Scenes");
		create_directories(project.GetPath().string() + "/Binaries");
		create_directories(project.GetPath().string() + "/Objs");

		CreateFileAndReplace(templatePath / "Project.cu.cut", project.GetPath() / "Project.cu", ":{ProjectName}", project.GetName());

		project.RegenerateBuildFiles();

		//Copy the Template Scene
		CopyFileTo(templatePath / "Assets/Scenes/EmptyTemplate.copper", project.GetAssetsPath() / "Scenes/EmptyTemplate.copper");
		CopyFileTo(templatePath / "Assets/Scenes/EmptyTemplate.copper.cum", project.GetAssetsPath() / "Scenes/EmptyTemplate.copper.cum");

        project.SetLastOpenedScenePath("Scenes/EmptyTemplate.copper");

		CopyFileTo("assets/ScriptAPI/Copper-ScriptingAPI.dll", project.GetPath() / "Binaries/Copper-ScriptingAPI.dll", true);

	}
	
	void CreateFileAndReplace(const fs::path& original, const fs::path& out, const std::string& what, const std::string& argument) {

		std::ifstream originalFile(original);
		std::ofstream templateFile(out);

		std::string line;
		while (std::getline(originalFile, line)) {

			size_t pos = line.find(what);
			while (pos != std::string::npos) {

				line.replace(pos, what.size(), argument);
				pos = line.find(what);

			}

			templateFile << line << "\n";

		}

		originalFile.close();
		templateFile.close();

	}

	void CopyFileTo(const fs::path& original, const fs::path& where, bool binary) {
		
		std::ios_base::openmode inFlags = std::ios_base::in;
		std::ios_base::openmode outFlags = std::ios_base::out;
		if(binary) {

			inFlags |= std::ios_base::binary;
			outFlags |= std::ios_base::binary;

		}

		std::ifstream originalFile;
		std::ofstream newFile;

		originalFile.open(original.string(), inFlags);
		newFile.open(where.string(), outFlags);

		newFile << originalFile.rdbuf();

		originalFile.close();
		newFile.close();

	}

}
