#include "cupch.h"
#include "Engine/Utilities/FileUtils.h"

#include "Engine/Core/Engine.h"

#include <commdlg.h>
#include <shlobj.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Copper::Utilities {

    Filesystem::Path OpenDialog(const std::string& title, const std::vector<std::string>& filters, const Filesystem::Path& initialDir) {

        LogError("Not implemented yet");

        return "";
        
    }

    Filesystem::Path SaveDialog(const std::string& title, const std::vector<std::string>& filters, const Filesystem::Path& initialDir) {

        LogError("Not implemented yet");

        return "";
        
    }

    Filesystem::Path FolderOpenDialog(const std::string& title, const Filesystem::Path& initialDir) {

        LogError("Not implemented yet");

        return "";

    }
    
}