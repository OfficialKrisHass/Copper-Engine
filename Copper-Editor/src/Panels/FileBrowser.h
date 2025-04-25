#pragma once

#include <Copper.h>

#include "Panels/Panel.h"

namespace Editor {

    class FileBrowser : public Panel {

    public:
        FileBrowser() : Panel("File Browser") {}

        void Initialize();

        static void SetRelativeDir(const Copper::fs::path& value) { m_projectRelativeDir = value; }
        static const Copper::fs::path& GetRelativeDir() { return m_projectRelativeDir; }

    private:
        static Copper::fs::path m_projectRelativeDir;

        virtual void UI() override;

        void RelativeDirHeader();
        void WindowPopup();
        void EntryPopup(const Copper::fs::path& path);

        void EntryIcon(const Copper::fs::path& path, bool directory);

        void DirectoryEntry(const Copper::fs::path& path, const std::string& filename);
        void FileEntry(const Copper::fs::path& path, const std::string& filename, const std::string& extension);

        void EditName(const Copper::fs::path& path, const std::string& filename);

        void NewScript(const Copper::fs::path& path);

    };

}
