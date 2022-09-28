﻿#include "cupch.h"
#include "Engine/Utilities/FileUtils.h"

#include "Engine/Core/Engine.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Copper::Utilities {

    std::string OpenDialog(const char* filter) {

        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*) GetWindow().GetWindowPtr());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        
        if (GetCurrentDirectoryA(256, currentDir)) ofn.lpstrInitialDir = currentDir;
        
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE) return ofn.lpstrFile;

        return std::string();
        
    }

    std::string SaveDialog(const char* filter) {

        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*) GetWindow().GetWindowPtr());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        
        if (GetCurrentDirectoryA(256, currentDir)) ofn.lpstrInitialDir = currentDir;
        
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
        
        ofn.lpstrDefExt = strchr(filter, '\0') + 1;

        if (GetSaveFileNameA(&ofn) == TRUE) return ofn.lpstrFile;
		
        return std::string();
        
    }
    
}