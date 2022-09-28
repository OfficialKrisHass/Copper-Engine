#pragma once

#include "Copper.h"
#include "Panels/Panel.h"

#include <filesystem>

namespace Editor {
    
    class FileBrowser : public Panel {
        
    public:
        FileBrowser() = default;
        FileBrowser(int test);

    private:
        std::filesystem::path currentDir;
        Copper::Texture directoryIcon;
        Copper::Texture fileIcon;
        
        virtual void UI() override;
    
    };
}