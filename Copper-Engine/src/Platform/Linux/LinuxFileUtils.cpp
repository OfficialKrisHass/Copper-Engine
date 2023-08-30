#include "cupch.h"
#include "Engine/Utilities/FileUtils.h"

#include "Engine/Renderer/Texture.h"

#include "Engine/Core/Engine.h"

#include "Engine/Filesystem/Path.h"
#include "Engine/Filesystem/Iterator.h"

#include "Engine/UI/ImGui.h"

#include <ImGui/imgui.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>
#include <portable-file-dialogs.h>

#define PADDING 16.0f
#define THUMBNAIL_SIZE 128.0f
#define CELL_SIZE (THUMBNAIL_SIZE + PADDING)

namespace Copper::Utilities {

    Filesystem::Path SaveDialog(const std::string& title, const std::vector<std::string>& filters, const Filesystem::Path& initialDir) {
        
        return pfd::save_file(title, initialDir, filters, pfd::opt::force_overwrite).result();

    }
	Filesystem::Path OpenDialog(const std::string& title, const std::vector<std::string>& filters, const Filesystem::Path& initialDir) {

        return pfd::open_file(title, initialDir, filters, pfd::opt::none).result()[0];

    }

	Filesystem::Path FolderOpenDialog(const std::string& title, const Filesystem::Path& initialDir) {

        return pfd::select_folder(title, initialDir).result();

    }

}