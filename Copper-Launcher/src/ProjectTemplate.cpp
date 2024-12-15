#include "ProjectTemplate.h"

#include "PersistentData.h"

#include "Dialogs.h"

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

            s_templates.push_back(ProjectTemplate(path.filename().string(), ""));

        }

    }

    void ProjectTemplate::CreateProject(const std::string& name, const fs::path& path) const {

        if (!fs::exists(path)) {

            Dialogs::Error("Invalid path", "The selected path path does not exist");
            return;

        }

        fs::path templatePath = PersistentData::EditorAssetsPath() / "Templates/Projects" / m_name;

        fs::create_directories(path / name / "Assets");
        fs::create_directories(path / name / "Binaries");

        FileFromTemplate(templatePath / "Project.cu.cut", path / name / "Project.cu", ":{ProjectName}", name);
        CopyFile(PersistentData::EditorAssetsPath() / "ScriptingAPI/Copper-ScriptingAPI.dll", path / name / "Binaries/Copper-ScriptingAPI.dll");

        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(templatePath / "Assets")) {

            if (entry.is_directory()) continue;

            const fs::path& original = entry.path();
            const fs::path& destination = path / name / fs::relative(original, templatePath);

            if (!fs::exists(destination.parent_path()))
                fs::create_directories(destination.parent_path());

            CopyFile(original, destination);

        }

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

        std::ios_base::openmode flags = (std::ios_base::openmode) ((binaryFile << 2) & std::ios_base::binary);

        std::ifstream in;
        std::ofstream out;

        in.open(inPath, std::ios_base::in | flags);
        out.open(outPath, std::ios_base::out | flags);

        out << in.rdbuf();

        in.close();
        out.close();

    }

}
