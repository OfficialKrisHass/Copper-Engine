#pragma once

#include <Copper.h>

namespace Editor::ModelLoader {

    using namespace Copper;

    void Load(const fs::path& path, const UUID& uuid);

}
