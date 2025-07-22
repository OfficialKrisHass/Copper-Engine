#include "cupch.h"
#include "Engine/Filesystem/DirWatch.h"
#include "Engine/Filesystem/WatchConstants.h"

#include <Windows.h>

#include <locale>
#include <codecvt>

#define FILTERS FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE

namespace Copper {

	void DirWatch::StartBackend() {

		CUP_FUNCTION();

		m_handle = CreateFileW(m_directory.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, HANDLE(0));
		CU_ASSERT(m_handle != nullptr, "Could not Create a File handle for DirWatch at directory '{}'. Error: '{}'", m_directory, GetLastError());

		m_closeEvent = CreateEventW(nullptr, true, false, nullptr);
		CU_ASSERT(m_closeEvent != nullptr, "Could not create the Close Event for DirWatch at directory '{}'. Error: '{}'", m_directory, GetLastError());

	}
	void DirWatch::StopBackend() {

		CUP_FUNCTION();

		CloseHandle(m_handle);
		CloseHandle(m_closeEvent);

	}

	void DirWatch::SendCloseEvent() {

		CUP_FUNCTION();

		SetEvent(m_closeEvent);

	}

	void DirWatch::MonitorChanges() {

		char buffer[BUFFER_SIZE];
		DWORD bytesRead = 0;

		OVERLAPPED overlapped = { 0 };
		overlapped.hEvent = CreateEventW(nullptr, true, false, nullptr);
		CU_ASSERT(overlapped.hEvent != nullptr, "Could not create Overlapped event for DirWatch at directory '{}'. Error: '{}'", m_directory, GetLastError());

		HANDLE events[2] = { overlapped.hEvent, m_closeEvent };
		bool asyncPending = false;

		std::vector<FileChange> parsedData;
		while (m_destroy == false) {

			// Windows does not give events like IN_RENAME_SELF or IN_DELETE_SELF so we need to periodically check
			// if the directory still exists. If it does not, we can assume it was renamed or moved or deleted.
			if (!fs::exists(m_directory)) {

				m_destroy = true;
				break;

			}

			parsedData.clear();

			// Start the asynchronous call

			BOOL ec = ReadDirectoryChangesW(m_handle, buffer, BUFFER_SIZE, false, FILTERS, &bytesRead, &overlapped, nullptr);
			CU_ASSERT(ec != 0, "DirWatch call to ReadDirectoryChangesW failed. Error: '{}', Directory: '{}'", GetLastError(), m_directory);

			asyncPending = true;

			// Wait for an event or timeout

			DWORD status = WaitForMultipleObjects(2, events, false, SLEEP_LENGTH);
			if (status == WAIT_FAILED) {

				LogError("DirWatch WaitForMultipleObjects failed. Error: '{}', Directory: '{}'", GetLastError(), m_directory);
				continue;

			} else if (status != WAIT_OBJECT_0) continue;

			// Handle the event

			BOOL success = GetOverlappedResult(m_handle, &overlapped, &bytesRead, true);
			CU_ASSERT(success != 0, "DirWatch call to GetOverlappedResult failed. Error: '{}', Directory: '{}'", GetLastError(), m_directory);
			CU_ASSERT(bytesRead != 0, "DirWatch call to ReadDirectoryChangesW read 0 bytes. Directory: '{}'", m_directory);

			asyncPending = false;

			FILE_NOTIFY_INFORMATION* fileInformation = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&buffer[0]);
			do {

				std::wstring nameW = std::wstring(fileInformation->FileName, fileInformation->FileNameLength / sizeof(fileInformation->FileName[0]));

				FileChangeType type = FileChangeType::None;
				switch (fileInformation->Action) {

				case FILE_ACTION_ADDED: type = FileChangeType::Created; break;
				case FILE_ACTION_MODIFIED: type = FileChangeType::Changed; break;
				case FILE_ACTION_REMOVED: type = FileChangeType::Deleted; break;
				case FILE_ACTION_RENAMED_OLD_NAME: type = FileChangeType::RenamedOld; break;
				case FILE_ACTION_RENAMED_NEW_NAME: type = FileChangeType::RenamedNew; break;
				default: break;

				}

				int32 size = WideCharToMultiByte(CP_UTF8, 0, nameW.c_str(), -1, nullptr, 0, nullptr, nullptr);
				std::string name = std::string(size, 0);
				WideCharToMultiByte(CP_UTF8, 0, nameW.c_str(), -1, &name[0], size, nullptr, nullptr);

				fs::path path = name;
				parsedData.emplace_back(path, type);

				if (fileInformation->NextEntryOffset == 0) break;
				fileInformation = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(fileInformation) + fileInformation->NextEntryOffset);

			} while (m_destroy == false);

			// Send data to main thread

			if (parsedData.empty()) continue;

			std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
			m_data.insert(m_data.end(), parsedData.begin(), parsedData.end());

		}

		// Cleanup in case we stopped mid processing

		if (asyncPending) {

			CancelIo(m_handle);
			GetOverlappedResult(m_handle, &overlapped, &bytesRead, true);

		}

		CloseHandle(overlapped.hEvent);

	}

}