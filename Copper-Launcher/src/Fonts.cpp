#include "Fonts.h"

#include "UI.h"

namespace Launcher::Fonts {

    ImFont* titleFont = nullptr;
    ImFont* subtitleFont = nullptr;

    ImFont* textFont = nullptr;
    ImFont* smallTextFont = nullptr;
    ImFont* bigTextFont = nullptr;

    void Initialize() {

        fs::path fontPath = ResourceDirectory() / MainFontPath;

        titleFont = UI::AddFont(fontPath, 40.0f);
        subtitleFont = UI::AddFont(fontPath, 35.0f);

        textFont = UI::AddFont(fontPath, 24.0f);
        smallTextFont = UI::AddFont(fontPath, 18.0f);
        bigTextFont = UI::AddFont(fontPath, 32.0f);

    }

    ImFont* TitleFont() { return titleFont; }
    ImFont* SubtitleFont() { return subtitleFont; }

    ImFont* TextFont() { return textFont; }
    ImFont* SmallTextFont() { return smallTextFont; }
    ImFont* BigTextFont() { return bigTextFont; }

}
