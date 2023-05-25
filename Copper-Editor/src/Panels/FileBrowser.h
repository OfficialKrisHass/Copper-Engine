#pragma once

#include "Copper.h"
#include "Panels/Panel.h"

namespace Editor {
    
    class FileBrowser : public Panel {
        
    public:
        FileBrowser() = default;
        FileBrowser(const Copper::Filesystem::Path& initialDir);

        static void SetRelativeDir(const Copper::Filesystem::Path& value) { projectRelativeDir = value; }

    private:
        static Copper::Filesystem::Path projectRelativeDir;
        
        virtual void UI() override;
    
    };
}