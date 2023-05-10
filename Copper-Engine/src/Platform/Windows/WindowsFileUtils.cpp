#include "cupch.h"
#include "Engine/Utilities/FileUtils.h"

#include "Engine/Core/Engine.h"

#include <commdlg.h>
#include <shlobj.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Copper::Utilities {

    Filesystem::Path OpenDialog(const char* filter, const Filesystem::Path& initialDir) {

        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*) GetWindow().GetWindowPtr());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        
        std::string initial = initialDir.CreateAndReplaceSeparator().String();
        ofn.lpstrInitialDir = initial.c_str();

        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE) return ofn.lpstrFile;

        return "";
        
    }

    Filesystem::Path SaveDialog(const char* filter, const Filesystem::Path& initialDir) {

        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*) GetWindow().GetWindowPtr());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        
        std::string initial = initialDir.CreateAndReplaceSeparator().String();
        ofn.lpstrInitialDir = initial.c_str();
        
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
        
        ofn.lpstrDefExt = strchr(filter, '\0') + 1;

        if (GetSaveFileNameA(&ofn) == TRUE) return ofn.lpstrFile;
		
        return "";
        
    }

    static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {

        if (uMsg == BFFM_INITIALIZED) {

            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
        }

        return 0;
    }

    Filesystem::Path FolderOpenDialog() {

        TCHAR path[256];

        BROWSEINFO bi = {0};
        bi.lpszTitle = L"Browse for folder...";
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        bi.lpfn = BrowseCallbackProc;

        LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

        if (pidl != 0) {
            
            SHGetPathFromIDList(pidl, path);

            IMalloc* imalloc = 0;

            if (SUCCEEDED(SHGetMalloc(&imalloc))) {

                imalloc->Free(pidl);
                imalloc->Release();

            }

            std::wstring w = path;
            return std::string(w.begin(), w.end());

        }

        return "";
    }
    
}