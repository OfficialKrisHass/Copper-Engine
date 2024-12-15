#pragma once

#include "Base.h"

#include <vector>

namespace Launcher {

    class ProjectTemplate {

    public:
        ProjectTemplate() = default;
        ProjectTemplate(const std::string& name, const std::string& description) : m_name(name), m_description(description) {}

        static void LoadTemplates();

        void CreateProject(const std::string& name, const fs::path& path) const;

        inline const std::string& GetName() const { return m_name; }
        inline const std::string& GetPath() const { return m_description; }

        inline static const std::vector<ProjectTemplate>& GetTemplates() { return s_templates; }

    private:
        std::string m_name = "";
        std::string m_description = "";

        static std::vector<ProjectTemplate> s_templates;

    };

}
