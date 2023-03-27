#include "WindowsHelpers.h"

#include <Windows.h>
#include <shlobj.h>

namespace Launcher::OS {

	void StartApplication(const std::filesystem::path& path, std::wstring arguments) {

		STARTUPINFO startupInfo;
		PROCESS_INFORMATION proccessInfo;

		ZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		ZeroMemory(&proccessInfo, sizeof(proccessInfo));

		CreateProcess(path.wstring().c_str(),
					  arguments.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startupInfo, &proccessInfo);

		CloseHandle(proccessInfo.hProcess);
		CloseHandle(proccessInfo.hThread);

	}

    static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	std::filesystem::path OpenFolderDialog() {

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
    static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {

        if (uMsg == BFFM_INITIALIZED) {

            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
        }

        return 0;
    }

}