#pragma once

#include <Copper.h>

namespace Editor {

    using namespace Copper;

    void SaveFields(const fs::path& path);
    void LoadFields(const fs::path& path);

}
