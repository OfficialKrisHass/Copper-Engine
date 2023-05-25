#include "cupch.h"
#include "Engine/Input/Popup.h"

#include <winuser.h>

namespace Copper::Input {

    PopupResult WinToCopper(int win) {

        switch(win) {

            case IDOK: return PopupResult::Ok; break;
            case IDCANCEL: return PopupResult::Cancel; break;

        }

    }
    
    PopupResult WarningPopup(const std::string& title, const std::string& description) {

        std::wstring wTitle = std::wstring(title.begin(), title.end());
        std::wstring wDescription = std::wstring(description.begin(), description.end());
        
        int ret = MessageBox(NULL, (LPCWSTR) wDescription.c_str(), (LPCWSTR) wTitle.c_str(), MB_ICONWARNING | MB_OKCANCEL);

        return WinToCopper(ret);
        
    }

    PopupResult ErrorPopup(const std::string& title, const std::string& description) {
        
        std::wstring wTitle = std::wstring(title.begin(), title.end());
        std::wstring wDescription = std::wstring(description.begin(), description.end());

        int ret = MessageBox(NULL, (LPCWSTR) wDescription.c_str(), (LPCWSTR) wTitle.c_str(), MB_ICONERROR | MB_OK);

        return WinToCopper(ret);

    }
    
}