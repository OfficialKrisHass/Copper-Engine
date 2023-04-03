#pragma once

#include "Copper.h"
#include "Panels/Panel.h"

#include <filesystem>

namespace Editor {
    
    class FileBrowser : public Panel {
        
    public:
        FileBrowser() = default;
        FileBrowser(const std::filesystem::path& initialDir);

        static void SetRelativeDir(const std::filesystem::path& value) { projectRelativeDir = value; }

    private:
        static std::filesystem::path projectRelativeDir;
        Copper::Texture directoryIcon;
        Copper::Texture fileIcon;
        
        virtual void UI() override;
    
    };
}