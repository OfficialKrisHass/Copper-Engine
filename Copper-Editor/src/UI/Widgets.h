#pragma once

#include <Copper.h>

namespace Copper { class Texture; }

namespace Editor::UI {

    using namespace Copper;

    void InfoLabel(const Texture& icon, const char* text);

}
