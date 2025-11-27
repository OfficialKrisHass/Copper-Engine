#pragma once

#include <Copper.h>

#include <Engine/Filesystem/RecursiveDirWatch.h>

namespace Editor {

    using namespace Copper;

    class Project {

    public:
        Project() = default;
        Project(const fs::path& path);

        std::string name;
        std::string description;

        void Create(const fs::path& path);

        void Open(const fs::path& path);
        void Open();
        void Save(bool saveScene = true) const;
        void SaveAs();

        void Update();

        void Build();

        void RegenerateProjectFiles() const;
        void RegenerateBuildFiles() const;

#ifdef CU_LINUX
        void RunPremake() const;
#endif

        // Getters

        inline const fs::path& GetPath() const { return m_path; }
        inline const fs::path GetAssetsPath() const { return m_path / "Assets"; }
        inline const fs::path& GetLastOpenedScenePath() const { return m_lastOpenedScenePath; }
        inline const std::string GetLastOpenedSceneName() const { return m_lastOpenedScenePath.filename().string(); }

        inline bool ShouldRebuild() const { return m_shouldRebuild; }

        // Setters

        inline void SetLastOpenedScenePath(const fs::path& value) { m_lastOpenedScenePath = value; }

        // Operators

        inline bool IsValid() const { return m_valid; }

    private:
        bool m_valid = false;

        fs::path m_path;
        fs::path m_lastOpenedScenePath;

        RecursiveDirWatch m_assetWatch;

        bool m_shouldRebuild = false;

        bool LoadFile(const Copper::fs::path& path);

        void FileChangeCallback(const fs::path& path, FileChangeType type, uint32 cookie);

    };

}
