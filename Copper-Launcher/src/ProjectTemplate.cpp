#include "ProjectTemplate.h"

#include "PersistentData.h"

namespace Launcher {

    std::vector<ProjectTemplate> ProjectTemplate::s_templates;

    void ProjectTemplate::LoadTemplates() {

        static const fs::path templatesPath = PersistentData::EditorAssetsPath() / "Templates/Projects";
        for (const fs::directory_entry& entry : fs::directory_iterator(templatesPath)) {

            const fs::path& path = entry.path();

            s_templates.push_back(ProjectTemplate(path.filename().string(), ""));

        }

    }

}
