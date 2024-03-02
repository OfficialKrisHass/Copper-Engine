#pragma once

#include "Copper.h"
#include "Panels/Panel.h"

#include "Engine/Core/Core.h"

namespace Editor {
    
    class FileBrowser : public Panel {
        
    public:
        FileBrowser() = default;
        FileBrowser(const Copper::fs::path& initialDir);

        static void SetRelativeDir(const Copper::fs::path& value) { m_projectRelativeDir = value; }

    private:
        static Copper::fs::path m_projectRelativeDir;
        
        virtual void UI() override;

        void RelativeDirHeader();
        void WindowPopup();
        void EntryPopup(const Copper::fs::path& path);

        void EntryIcon(bool directory);

        void DirectoryEntry(const Copper::fs::path& path, const std::string& filename);
        void FileEntry(const Copper::fs::path& path, const std::string& filename, const std::string& extension);

        void EditName(const Copper::fs::path& path, const std::string& filename);
    
    };
}