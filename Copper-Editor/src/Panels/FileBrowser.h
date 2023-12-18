#pragma once

#include "Copper.h"
#include "Panels/Panel.h"

#include "Engine/Core/Core.h"

namespace Editor {
    
    class FileBrowser : public Panel {
        
    public:
        FileBrowser() = default;
        FileBrowser(const fs::path& initialDir);

        static void SetRelativeDir(const fs::path& value) { projectRelativeDir = value; }

    private:
        static fs::path projectRelativeDir;
        
        virtual void UI() override;
    
    };
}