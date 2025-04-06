#pragma once

#include <Copper.h>

#include "Panels/Panel.h"

struct ImGuiStyle;

namespace Editor {

    class ThemeEditor : public Panel {

    public:
        ThemeEditor() : Panel("Theme Editor") {}

        void SaveTheme(const Copper::fs::path& path);
        void LoadTheme(const Copper::fs::path& path);

    private:
        virtual void UI() override;

        void ColorEditor();

        ImGuiStyle* m_style = nullptr;

    };

}
