#include "cupch.h"
#include "Engine/Input/Error.h"

#include <winuser.h>

namespace Copper::Input::Error {
    
    int WarningPopup(std::string title, std::string description) {

        std::wstring wTitle = std::wstring(title.begin(), title.end());
        std::wstring wDescription = std::wstring(description.begin(), description.end());
        
        int ret = MessageBox(NULL, (LPCWSTR) wDescription.c_str(), (LPCWSTR) wTitle.c_str(), MB_ICONWARNING | MB_OKCANCEL);

        return ret;
        
    }

    int ErrorPopup(std::string title, std::string description) {
        
        std::wstring wTitle = std::wstring(title.begin(), title.end());
        std::wstring wDescription = std::wstring(description.begin(), description.end());

        int ret = MessageBox(NULL, (LPCWSTR) wDescription.c_str(), (LPCWSTR) wTitle.c_str(), MB_ICONERROR | MB_OK);

        return ret;

    }
    
}