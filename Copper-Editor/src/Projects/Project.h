#pragma once

#include <Copper.h>

namespace Editor {

    class Project {

    public:
        Project() = default;
        Project(const Copper::fs::path& path);

        void Open(const Copper::fs::path& path);
        void Open();
        void Save(bool saveScene = true) const;
        void SaveAs();

        bool BuildScripts() const;

        void RegenerateProjectFiles() const;
        void RegenerateBuildFiles() const;

#ifdef CU_LINUX
        void RunPremake() const;
#endif

        // Getters

        inline const std::string& GetName() const { return m_name; }

        inline const Copper::fs::path& GetPath() const { return m_path; }
        inline const Copper::fs::path GetAssetsPath() const { return m_path / "Assets"; }
        inline const Copper::fs::path& GetLastOpenedScenePath() const { return m_lastOpenedScenePath; }
        inline const std::string GetLastOpenedSceneName() const { return m_lastOpenedScenePath.filename().string(); }

        // Setters

        inline void SetName(const std::string& value) { m_name = value; }

        inline void SetLastOpenedScenePath(const Copper::fs::path& value) { m_lastOpenedScenePath = value; }

        // Operators

        inline bool IsValid() const { return m_valid; }

    private:
        bool m_valid = false;
        std::string m_name;

        Copper::fs::path m_path;
        Copper::fs::path m_lastOpenedScenePath;

        bool LoadFile(const Copper::fs::path& path);

    };

}
