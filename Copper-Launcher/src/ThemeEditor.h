#pragma once

struct ImVec4;

namespace Launcher {

    void ThemeEditor();

    void SaveTheme();
    void LoadTheme();

    const ImVec4& ColorDisabled();

}
