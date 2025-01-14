#pragma once

#include "Base.h"

namespace Launcher {

    class ProjectEntry {

    public:
        ProjectEntry(const std::string& name, const fs::path& directory) : m_name(name), m_directory(directory) {}

        static void InitializeFonts();

        void Render() const;

        inline const std::string& Name() const { return m_name; }
        inline const fs::path& Directory() const { return m_directory; }

    private:
        std::string m_name = "";
        fs::path m_directory = "";

    };

}
