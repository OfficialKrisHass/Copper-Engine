#include "ProjectTemplate.h"

#include "PersistentData.h"

#include "Dialogs.h"

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <iostream>

#include <cstring>

namespace Launcher {

    std::vector<ProjectTemplate> ProjectTemplate::s_templates;

    static void FileFromTemplate(const fs::path& templatePath, const fs::path& outPath, const std::string& argument, const std::string& value);
    static void CopyFile(const fs::path& inPath, const fs::path& outPath, bool binaryFile = false);

    void ProjectTemplate::LoadTemplates() {

        static const fs::path templatesPath = PersistentData::EditorAssetsPath() / "Templates/Projects";
        for (const fs::directory_entry& entry : fs::directory_iterator(templatesPath)) {

            const fs::path& path = entry.path();

            YAML::Node node;
            try { node = YAML::LoadFile((path / "Project.cu.cut").string()); }
            catch (YAML::Exception& e) {

                std::cerr << "Could not read Template '" << path.filename().string() << "' Project.cu.cut file!\n";
                std::cerr << "Error message: " << e.what() << "\n";

                continue;

            }

            s_templates.push_back(ProjectTemplate(path.filename().string(), node["Description"].as<std::string>()));

        }

    }

    bool ProjectTemplate::CreateProject(const std::string& name, const fs::path& path) const {

        if (!fs::exists(path)) {

            Dialogs::Error("Invalid path", "The selected path path does not exist");
            return false;

        }

        fs::path templatePath = PersistentData::EditorAssetsPath() / "Templates/Projects" / m_name;

        fs::create_directories(path / name / "Assets");
        fs::create_directories(path / name / "Binaries");

        FileFromTemplate(templatePath / "Project.cu.cut", path / name / "Project.cu", ":{ProjectName}", name);
        CopyFile(templatePath / "ProjectMetadata.cu.cut", path / name / "ProjectMetadata.cu");

        CopyFile(PersistentData::EditorAssetsPath() / "Copper-ScriptingAPI.dll", path / name / "Binaries/Copper-ScriptingAPI.dll", true);

        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(templatePath / "Assets")) {

            if (entry.is_directory()) continue;

            const fs::path& original = entry.path();
            const fs::path& destination = path / name / fs::relative(original, templatePath);

            if (!fs::exists(destination.parent_path()))
                fs::create_directories(destination.parent_path());

            CopyFile(original, destination);

        }

        GenerateBuildFiles(name, path);
        BuildProject(name, path);
        
        return true;

    }

    void ProjectTemplate::GenerateBuildFiles(const std::string& name, const fs::path& path) const {

        const fs::path templateDir = PersistentData::EditorAssetsPath() / "Templates/Project Files";

#ifdef CU_WINDOWS
        FileFromTemplate(templateDir / "Template.sln.cut", path / name / (name + ".sln"), ":{ProjectName}", name);
        FileFromTemplate(templateDir / "Template.csproj.cut", path / name / (name + ".csproj"), ":{ProjectName}", name);
#elif CU_LINUX
        FileFromTemplate(templateDir / "premake5.lua.cut", path / name / "premake5.lua", ":{ProjectName}", name);

        const std::string premakePath = PersistentData::EditorAssetsPath() / "../util/premake/premake5";
        const std::string file = path / name / "premake5.lua";

        const std::string cmd = premakePath + " --file=\"" + file + "\" gmake2";
        system(cmd.c_str());
#endif

    }
    void ProjectTemplate::BuildProject(const std::string& name, const fs::path& path) const {

#ifdef CU_WINDOWS
        std::string cmd = "C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\MSBuild.exe ";

        size_t pos = path.string().find_first_of(' ');
        std::string newPath = path.string();
        while (pos != std::string::npos) {

            newPath.erase(pos, 1);
            newPath.insert(pos, "\" \"");
            pos = newPath.find_first_of(' ', pos + 3);

        }

        pos = name.find_first_of(' ');
        std::string newName = name;
        while (pos != std::string::npos) {

            newName.erase(pos, 1);
            newName.insert(pos, "\" \"");
            pos = newName.find_first_of(' ', pos + 3);

        }

        cmd += newPath + "\\" + newName + "\\" + newName + ".csproj";
        cmd += " -nologo";

        system(cmd.c_str());
#elif CU_LINUX
        const std::string makefileDir = path / name;
        const std::string cmd = "make --no-print-directory -C \"" + makefileDir + "\" -f Makefile";

        system(cmd.c_str());
#endif

    }

    static void FileFromTemplate(const fs::path& templatePath, const fs::path& outPath, const std::string& argument, const std::string& value) {

        std::ifstream templ;
        std::ofstream out;

        templ.open(templatePath);
        out.open(outPath);

        std::string line;
        while (std::getline(templ, line)) {

            size_t begin = line.find(argument);
            while (begin != std::string::npos) {

                line.replace(begin, argument.length(), value);
                begin = line.find(argument, begin + 1);
            
            }

            out << line << "\n";


        }

        templ.close();
        out.close();

    }
    static void CopyFile(const fs::path& inPath, const fs::path& outPath, bool binaryFile) {

        std::ios_base::openmode flags = 0;
        if (binaryFile)
            flags = std::ios_base::binary;

        std::ifstream in;
        std::ofstream out;

        in.open(inPath, std::ios_base::in | flags);
        out.open(outPath, std::ios_base::out | flags);

        out << in.rdbuf();

        in.close();
        out.close();

    }

}
