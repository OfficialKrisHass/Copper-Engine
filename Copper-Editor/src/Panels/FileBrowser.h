#pragma once

#include <Copper.h>

#include "Panels/Panel.h"

namespace Editor {

    using namespace Copper;

    class FileBrowser : public Panel {

    public:
        FileBrowser() : Panel("File Browser") {}

        void Initialize();
        static void Refresh();

        static void SetRelativeDir(const Copper::fs::path& value) { m_projectRelativeDir = value; }
        static const Copper::fs::path& GetRelativeDir() { return m_projectRelativeDir; }

    private:
        struct DirectoryEntry {

            DirectoryEntry* parent = nullptr;
            std::unordered_map<std::string, DirectoryEntry> folders;
            std::vector<std::string> files;

        };

        static DirectoryEntry m_rootEntry;
        static const DirectoryEntry* m_currEntry;

        static fs::path m_projectRelativeDir;

        virtual void UI() override;

        void RenderDirectoryEntry(const DirectoryEntry& entry);
        static void RefreshDirectoryTree(DirectoryEntry& entry, fs::path path);

        void RelativeDirHeader();
        void WindowPopup();
        void EntryPopup(const Copper::fs::path& path);

        void EditName(const Copper::fs::path& path, const std::string& filename);

    };

}
