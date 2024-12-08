#include "ProjectTemplate.h"

#include <fstream>

using std::filesystem::create_directories;

namespace Editor {

    using namespace Copper;

    void CreateFileAndReplace(const fs::path& original, const fs::path& out, const std::string& what, const std::string& replace);

    void CopyFileTo(const fs::path& original, const fs::path& where, bool binary = false);

    void CreateTemplateFromProject(const Project& project) {

        CUP_FUNCTION();

        const fs::path path = ExecutableFolder() + "/assets/Templates/" + project.GetName() + "_Template";

        create_directories(path.string() + "/Assets");
        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(project.GetAssetsPath())) {

            const fs::path& filePath = entry.path();
            if (!fs::exists(filePath.parent_path()))
                fs::create_directories(filePath.parent_path());

            CopyFileTo(filePath, path / fs::relative(filePath, project.GetPath()));

        }

        CreateFileAndReplace(project.GetPath() / "Project.cu", path / "Project.cu.cut", project.GetName(), ":{ProjectName}");

    }
    void CreateProjectFromTemplate(const fs::path& templatePath, Project& project) {

        CUP_FUNCTION();

        const fs::path templ = ExecutableFolder() / templatePath;
        Log("Creating a project from template at path: {}", templ.string());

        create_directories(project.GetAssetsPath());
        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(templ)) {

            const fs::path& path = entry.path();
            const fs::path& destination = project.GetPath() / fs::relative(path, templ);

            if (!fs::exists(destination.parent_path()))
                fs::create_directories(destination.parent_path());

            CopyFileTo(path, destination);

        }
        CreateFileAndReplace(templatePath / "Project.cu.cut", project.GetPath() / "Project.cu", ":{ProjectName}", project.GetName());
        CopyFileTo(ExecutableFolder() + "/assets/ScriptingAPI/Copper-ScriptingAPI.dll", project.GetPath() / "Binaries/Copper-ScriptingAPI.dll", true);

        project.RegenerateBuildFiles();

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
