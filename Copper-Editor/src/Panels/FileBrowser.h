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
    
    };
}