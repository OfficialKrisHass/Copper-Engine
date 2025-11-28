#include "cupch.h"
#include "Engine/Filesystem/FileWatch.h"
#include "Engine/Filesystem/WatchConstants.h"

#include <Windows.h>

#include <locale>
#include <codecvt>

#define FILTERS FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE

namespace Copper {

	void FileWatch::StartBackend() {

		CUP_FUNCTION();

		m_handle = CreateFileW(m_path.parent_path().c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, HANDLE(0));
		CU_ASSERT(m_handle != nullptr, "Could not Create a File handle for FileWatch at path '{}'. Error: '{}'", m_path, GetLastError());

		m_closeEvent = CreateEventW(nullptr, true, false, nullptr);
		CU_ASSERT(m_closeEvent != nullptr, "Could not create the Close Event for FileWatch at path '{}'. Error: '{}'", m_path, GetLastError());

	}
	void FileWatch::StopBackend() {

		CloseHandle(m_handle);
		CloseHandle(m_closeEvent);

	}

	void FileWatch::SendCloseEvent() {

		SetEvent(m_closeEvent);

	}

	void FileWatch::MonitorChanges() {

		char buffer[BUFFER_SIZE];
		DWORD bytesRead = 0;

		OVERLAPPED overlapped = { 0 };
		overlapped.hEvent = CreateEventW(nullptr, true, false, nullptr);
		CU_ASSERT(overlapped.hEvent != nullptr, "Could not create Overlapped event for FileWatch at path '{}'. Error: '{}'", m_path, GetLastError());

		HANDLE events[2] = { overlapped.hEvent, m_closeEvent };
		bool asyncPending = false;

		std::vector<FileChangeType> parsedData;
		while (m_destroy == false) {

			parsedData.clear();

			BOOL ec = ReadDirectoryChangesW(m_handle, buffer, BUFFER_SIZE, false, FILTERS, &bytesRead, &overlapped, nullptr);
			CU_ASSERT(ec != 0, "FileWatch call to ReadDirectoryChangesW failed. Error: '{}', Path: '{}'", GetLastError(), m_path);

			asyncPending = true;

			switch (WaitForMultipleObjects(2, events, false, SLEEP_LENGTH)) {

			case WAIT_OBJECT_0: {

				// File Change event

				BOOL success = GetOverlappedResult(m_handle, &overlapped, &bytesRead, true);
				CU_ASSERT(success != 0, "FileWatch call to GetOverlappedResult failed. Error: '{}', Path: '{}'", GetLastError(), m_path);
				CU_ASSERT(bytesRead != 0, "FileWatch call to ReadDirectoryChangesW read 0 bytes. Path: '{}'", m_path);

				asyncPending = false;

				FILE_NOTIFY_INFORMATION* fileInformation = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&buffer[0]);
				do {

					std::wstring nameW = std::wstring(fileInformation->FileName, fileInformation->FileNameLength / sizeof(fileInformation->FileName[0]));
					if (nameW != m_path.filename().c_str()) {

						if (fileInformation->NextEntryOffset == 0) break;
						fileInformation = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(fileInformation) + fileInformation->NextEntryOffset);

						continue;

					}

					FileChangeType type = FileChangeType::None;
					switch (fileInformation->Action) {

					case FILE_ACTION_ADDED: type = FileChangeType::Created; break;
					case FILE_ACTION_MODIFIED: type = FileChangeType::Changed; break;
					case FILE_ACTION_REMOVED: {

						type = FileChangeType::Deleted;
						m_destroy = true;

						break;

					}
					case FILE_ACTION_RENAMED_OLD_NAME: {

						type = FileChangeType::RenamedOld;
						m_destroy = true;

						break;

					}
					case FILE_ACTION_RENAMED_NEW_NAME: type = FileChangeType::RenamedNew; break;
					default: break;

					}

					parsedData.emplace_back(type);

					if (fileInformation->NextEntryOffset == 0) break;
					fileInformation = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(fileInformation) + fileInformation->NextEntryOffset);

				} while (m_destroy == false);

				break;

			}
			case WAIT_FAILED: {

				LogError("FileWatch WaitForMultipleObjects failed. Error: '{}', Path: '{}'", GetLastError(), m_path);

				break;

			}
			default: break;

			}

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