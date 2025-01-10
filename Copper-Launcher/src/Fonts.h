#pragma once

#include "Base.h"

struct ImFont;

namespace Launcher::Fonts {

    void Initialize();

    ImFont* TitleFont();
    ImFont* SubtitleFont();

    ImFont* TextFont();
    ImFont* SmallTextFont();
    ImFont* BigTextFont();

}
