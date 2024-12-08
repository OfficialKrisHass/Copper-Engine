#include "Fonts.h"

#include "UI.h"

namespace Launcher::Fonts {

    ImFont* titleFont = nullptr;
    ImFont* subtitleFont = nullptr;

    ImFont* textFont = nullptr;
    ImFont* smallTextFont = nullptr;
    ImFont* bigTextFont = nullptr;

    void Initialize() {

        titleFont = UI::AddFont(ExecutableFolder() + MainFontPath, 40.0f);
        subtitleFont = UI::AddFont(ExecutableFolder() + MainFontPath, 35.0f);

        textFont = UI::AddFont(ExecutableFolder() + MainFontPath, 24.0f);
        smallTextFont = UI::AddFont(ExecutableFolder() + MainFontPath, 18.0f);
        bigTextFont = UI::AddFont(ExecutableFolder() + MainFontPath, 32.0f);

    }

    ImFont* TitleFont() { return titleFont; }
    ImFont* SubtitleFont() { return subtitleFont; }

    ImFont* TextFont() { return textFont; }
    ImFont* SmallTextFont() { return smallTextFont; }
    ImFont* BigTextFont() { return bigTextFont; }

}
