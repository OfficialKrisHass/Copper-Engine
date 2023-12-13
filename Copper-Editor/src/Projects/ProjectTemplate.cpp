#include "ProjectTemplate.h"

#ifdef CU_LINUX
using std::experimental::filesystem::create_directories;
#else
using std::filesystem::create_directories;
#endif

namespace Editor {

	void CreateFileAndReplace(const Filesystem::Path& original, const Filesystem::Path& out, const std::string& what, const std::string& replace);

	void CopyFileTo(const Filesystem::Path& original, const Filesystem::Path& where, bool binary = false);

	void CreateTemplateFromProject(const Project& project) {

		const Filesystem::Path path = "assets/Templates/" + project.name;

		create_directories(path.String() + "/Assets/Scenes");

		CreateFileAndReplace(project.path / "Project.cu", path / "Project.cu.cut", project.name, ":{ProjectName}");

		CopyFileTo(project.assetsPath / project.lastOpenedScene, path / "Assets" / project.lastOpenedScene);
		CopyFileTo(project.assetsPath / (project.lastOpenedScene.String() + ".cum"), path / "Assets" / (project.lastOpenedScene.String() + ".cum"));

	}
	void CreateProjectFromTemplate(const Filesystem::Path& templatePath, Project& project) {

		create_directories(project.path.String() + "/Assets/Scenes");
		create_directories(project.path.String() + "/Binaries");
		create_directories(project.path.String() + "/Objs");

		CreateFileAndReplace(templatePath / "Project.cu.cut", project.path / "Project.cu", ":{ProjectName}", project.name);

		project.RegenerateIDEFiles();

		//Copy the Template Scene
		CopyFileTo(templatePath / "Assets/Scenes/EmptyTemplate.copper", project.assetsPath / "Scenes/EmptyTemplate.copper");
		CopyFileTo(templatePath / "Assets/Scenes/EmptyTemplate.copper.cum", project.assetsPath / "Scenes/EmptyTemplate.copper.cum");

		project.lastOpenedScene = "Scenes/EmptyTemplate.copper";

		CopyFileTo("assets/ScriptAPI/Copper-ScriptingAPI.dll", project.path / "Binaries/Copper-ScriptingAPI.dll", true);

	}
	
	void CreateFileAndReplace(const Filesystem::Path& original, const Filesystem::Path& out, const std::string& what, const std::string& argument) {

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

	void CopyFileTo(const Filesystem::Path& original, const Filesystem::Path& where, bool binary) {
		
		std::ios_base::openmode inFlags = std::ios_base::in;
		std::ios_base::openmode outFlags = std::ios_base::out;
		if(binary) {

			inFlags |= std::ios_base::binary;
			outFlags |= std::ios_base::binary;

		}

		std::ifstream originalFile;
		std::ofstream newFile;

		originalFile.open(original.String(), inFlags);
		newFile.open(where.String(), outFlags);

		newFile << originalFile.rdbuf();

		originalFile.close();
		newFile.close();

	}

}