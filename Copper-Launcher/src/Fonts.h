#pragma once

#include "Base.h"

class ImFont;

namespace Launcher::Fonts {

    void Initialize();

    ImFont* TitleFont();
    ImFont* SubtitleFont();

    ImFont* TextFont();
    ImFont* SmallTextFont();
    ImFont* BigTextFont();

}
