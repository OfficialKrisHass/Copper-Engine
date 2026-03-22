#include "ProjectTemplate.h"

#include <fstream>

using std::filesystem::create_directories;

namespace Editor {

    using namespace Copper;

    void CreateFileAndReplace(const fs::path& original, const fs::path& out, const std::string& what, const std::string& replace);

    void CopyFileTo(const fs::path& original, const fs::path& where, bool binary = false);

    void CreateTemplateFromProject(const Project& project) {

        CUP_FUNCTION();

        const fs::path path = ResourceDirectory() / "assets/Templates/Projects" / project.name;

        create_directories(path.string() + "/Assets");
        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(project.GetAssetsPath())) {

            const fs::path& relPath = fs::relative(entry.path(), project.GetPath());
            const fs::path tmplPath = path / relPath;

            if (!fs::exists(tmplPath.parent_path()))
                fs::create_directories(tmplPath.parent_path());

            CopyFileTo(project.GetPath() / relPath, tmplPath);

        }

        CreateFileAndReplace(project.GetPath() / "Project.cu", path / "Project.cu.cut", project.name, ":{ProjectName}");
        CopyFileTo(project.GetPath() / "ProjectMetadata.cu", path / "ProjectMetadata.cu.cut");

    }
    void CreateProjectFromTemplate(const std::string& templateName, Project& project) {

        CUP_FUNCTION();

        const fs::path templ = ResourceDirectory() / "assets/Templates/Projects" / templateName;

        create_directories(project.GetAssetsPath());
        create_directories((project.GetPath() / "Binaries"));

        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(templ / "Assets")) {

            const fs::path& path = entry.path();
            const fs::path& destination = project.GetPath() / fs::relative(path, templ);

            if (!fs::exists(destination.parent_path()))
                fs::create_directories(destination.parent_path());

            CopyFileTo(path, destination);

        }

        CreateFileAndReplace(templ / "Project.cu.cut", project.GetPath() / "Project.cu", ":{ProjectName}", project.name);
        CopyFileTo(templ / "ProjectMetadata.cu.cut", project.GetPath() / "ProjectMetadata.cu");

        CopyFileTo(ResourceDirectory() / "assets/Copper-ScriptingAPI.dll", project.GetPath() / "Binaries/Copper-ScriptingAPI.dll", true);

        project.RegenerateBuildFiles();

    }
    
    void CreateFileAndReplace(const fs::path& original, const fs::path& out, const std::string& what, const std::string& argument) {

        CUP_FUNCTION();

        CU_ASSERT(fs::exists(original), "Can't copy a file that does not exist. Path: '{}'", original);
        if (!fs::exists(out.parent_path()))
            fs::create_directories(out.parent_path());

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

        CUP_FUNCTION();

        CU_ASSERT(fs::exists(original), "Can't copy a file that does not exist. Path: '{}'", original);
        
        std::ios_base::openmode inFlags = std::ios_base::in;
        std::ios_base::openmode outFlags = std::ios_base::out;
        if(binary) {

            inFlags |= std::ios_base::binary;
            outFlags |= std::ios_base::binary;

        }

        std::ifstream originalFile;
        std::ofstream newFile;

        if (!fs::exists(where.parent_path()))
            fs::create_directories(where.parent_path());

        originalFile.open(original.string(), inFlags);
        newFile.open(where.string(), outFlags);

        newFile << originalFile.rdbuf();

        originalFile.close();
        newFile.close();

    }

}
