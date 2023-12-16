#include "cupch.h"
#include "Engine/Utilities/FileUtils.h"

#include "Engine/Core/Engine.h"

#include <portable-file-dialogs.h>

namespace Copper::Utilities {

    Filesystem::Path OpenDialog(const std::string& title, const std::vector<std::string>& filters, const Filesystem::Path& initialDir) {

        return pfd::open_file(title, initialDir.String(), filters, false).result()[0];
        
    }

    Filesystem::Path SaveDialog(const std::string& title, const std::vector<std::string>& filters, const Filesystem::Path& initialDir) {

        return pfd::save_file(title, initialDir.String(), filters).result();
        
    }

    Filesystem::Path FolderOpenDialog(const std::string& title, const Filesystem::Path& initialDir) {

        return pfd::select_folder(title, initialDir.String()).result();

    }
    
}