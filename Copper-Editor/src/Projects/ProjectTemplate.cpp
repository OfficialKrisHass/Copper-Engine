#include "ProjectTemplate.h"

namespace Editor {

	void CreateFileAndReplace(const Filesystem::Path& originalPath, const Filesystem::Path& outPath, const std::string& what, const std::string& replace);

	void CopyFileTo(const Filesystem::Path& original, const Filesystem::Path& where, bool binary = false);

	void CreateTemplateFromProject(const Project& project) {

		//Create The Directories
		std::experimental::filesystem::create_directories("assets\\Templates\\" + project.name + "\\Assets\\Scenes");
		std::experimental::filesystem::create_directories("assets\\Templates\\" + project.name + "\\Binaries");
		std::experimental::filesystem::create_directories("assets\\Templates\\" + project.name + "\\Objs");

		//Create the .cu, .sln, .csproj template files
		CreateFileAndReplace(project.path / "Project.cu", "assets\\Templates\\" + project.name + "\\Project.cu.cut", project.name, ":{ProjectName}");
		CreateFileAndReplace(project.path / "DevProject.sln", "assets\\Templates\\" + project.name + "\\Template.sln.cut", project.name, ":{ProjectName}");
		CreateFileAndReplace(project.path / "DevProject.csproj", "assets\\Templates\\" + project.name + "\\Template.csproj.cut", project.name, ":{ProjectName}");

		//Copy the Last opened Scene
		CopyFileTo(project.assetsPath / project.lastOpenedScene, "assets\\Templates\\" + project.name + "\\Assets\\" + project.lastOpenedScene.String());
		CopyFileTo(project.assetsPath / (project.lastOpenedScene.String() + ".cum"), "assets\\Templates\\" + project.name + "\\Assets\\" + project.lastOpenedScene.String() + ".cum");

		//Copy the Scripting API dll
		CopyFileTo("assets\\ScriptAPI\\Copper-ScriptingAPI.dll", "assets\\Templates\\" + project.name + "\\Binaries\\Copper-ScriptingAPI.dll", true);

	}
	void CreateProjectFromTemplate(const Filesystem::Path& templatePath, Project& project) {

		//Create the Directories
		std::experimental::filesystem::create_directories((project.path / "Assets\\Scenes").String());
		std::experimental::filesystem::create_directories((project.path / "Binaries").String());
		std::experimental::filesystem::create_directories((project.path / "Objs").String());

		//Create the Project files from the tempalte
		CreateFileAndReplace(templatePath / "Project.cu.cut", project.path / "Project.cu", ":{ProjectName}", project.name);
		CreateFileAndReplace(templatePath / "Template.sln.cut", project.path / (project.name + ".sln"), ":{ProjectName}", project.name);
		CreateFileAndReplace(templatePath / "Template.csproj.cut", project.path / (project.name + ".csproj"), ":{ProjectName}", project.name);

		//Copy the Template Scene
		CopyFileTo(templatePath / "Assets\\Scenes\\EmptyTemplate.copper", project.assetsPath / "Scenes\\EmptyTemplate.copper");
		CopyFileTo(templatePath / "Assets\\Scenes\\EmptyTemplate.copper.cum", project.assetsPath / "Scenes\\EmptyTemplate.copper.cum");

		project.lastOpenedScene = "Scenes\\EmptyTemplate.copper";

		////Copy the Scripting API dll
		CopyFileTo("assets\\ScriptAPI\\Copper-ScriptingAPI.dll", project.path / "Binaries\\Copper-ScriptingAPI.dll", true);

	}
	
	void CreateFileAndReplace(const Filesystem::Path& originalPath, const Filesystem::Path& outPath, const std::string& what, const std::string& argument) {

		std::ifstream originalFile(originalPath);
		std::ofstream templateFile(outPath);

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