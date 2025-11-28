#include "cupch.h"
#include "Engine/Filesystem/RecursiveDirWatch.h"
#include "Engine/Filesystem/WatchConstants.h"

#include <Windows.h>

#include <random>
#include <chrono>

#include <locale>
#include <codecvt>

#define FILTERS FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_ACCESS | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY

// I HATE HATE HATE HATE HATE WINDOWS
// THE WINDOWS API IS ONE OF THE WORST APIS ON PLANET EARTH
//
// Windows will randomly decide to generate 2 or 3 events when a file is deleted, 2 when created, change events for folders
// all absolutely lovely to handle. A lot of the code will throw errors because an asset will be attempted to be removed 3 times
// but I do not fucking care, go to bill g and tell him to fix his stupid dumbass code.

namespace Copper {

	typedef std::chrono::steady_clock Clock;

	struct RenameInfo {

		fs::path path;
		FileChangeType type = FileChangeType::None;

		uint32 cookie = 0;
		Clock::time_point timestamp;

		RenameInfo(const fs::path& path, FileChangeType type, uint32 cookie, Clock::time_point timestamp)
			: path(path), type(type), cookie(cookie), timestamp(timestamp) {}

	};
	std::unordered_map<std::string, RenameInfo> renameMap;

	std::random_device random;

	void WStringToString(const std::wstring& wide, std::string& out);

	void RecursiveDirWatch::StartBackend() {

		CUP_FUNCTION();

		m_handle = CreateFileW(m_directory.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, HANDLE(0));
		CU_ASSERT(m_handle != nullptr, "Could not Create a File handle for RecursiveDirWatch at directory '{}'. Error: '{}'", m_directory, GetLastError());

		m_closeEvent = CreateEventW(nullptr, true, false, nullptr);
		CU_ASSERT(m_closeEvent != nullptr, "Could not create the Close Event for RecursiveDirWatch at directory '{}'. Error: '{}'", m_directory, GetLastError());

	}
	void RecursiveDirWatch::StopBackend() {

		CloseHandle(m_handle);
		CloseHandle(m_closeEvent);

	}

	void RecursiveDirWatch::SendCloseEvent() {

		SetEvent(m_closeEvent);

	}

	void RecursiveDirWatch::MonitorChanges() {

		char buffer[BUFFER_SIZE] = {};
		DWORD bytesRead = 0;

		OVERLAPPED overlapped = { 0 };
		overlapped.hEvent = CreateEventW(nullptr, true, false, nullptr);
		CU_ASSERT(overlapped.hEvent != nullptr, "Could not create Overlapped event for RecursiveDirWatch at directory '{}'. Error: '{}'", m_directory, GetLastError());

		HANDLE events[2] = { overlapped.hEvent, m_closeEvent };
		bool asyncPending = false;

		std::vector<FileChange> parsedData;
		while (m_running) {

			parsedData.clear();

			// Resolve unpaired events in renameMap

			Clock::time_point now = Clock::now();
			for (auto it = renameMap.begin(); it != renameMap.end();) {

				RenameInfo& info = it->second;
				if (now - info.timestamp >= std::chrono::milliseconds(SLEEP_LENGTH * 2)) {

					FileChangeType type = FileChangeType::None;
					if (info.type == FileChangeType::RenamedNew)
						type = FileChangeType::Created;
					else
						type = FileChangeType::Deleted;

					parsedData.emplace_back(info.path, type, 0);
					it = renameMap.erase(it);

					continue;

				}

				++it;

			}
			if (!parsedData.empty()) {

				std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
				m_data.insert(m_data.end(), parsedData.begin(), parsedData.end());

			}

			// Start the asynchronous call

			if (!asyncPending) {

				BOOL ec = ReadDirectoryChangesW(m_handle, buffer, BUFFER_SIZE, true, FILTERS, &bytesRead, &overlapped, nullptr);
				CU_ASSERT(ec != 0, "RecursiveDirWatch call to ReadDirectoryChangesW failed. Error: '{}', Directory: '{}'", GetLastError(), m_directory);

				asyncPending = true;

			}

			// Wait for an event or timeout

			DWORD status = WaitForMultipleObjects(2, events, false, SLEEP_LENGTH);
			if (status == WAIT_FAILED) {

				LogError("RecursiveDirWatch WaitForMultipleObjects failed. Error: '{}', Directory: '{}'", GetLastError(), m_directory);
				continue;

			}
			else if (status != WAIT_OBJECT_0) continue;

			// Handle the event

			BOOL success = GetOverlappedResult(m_handle, &overlapped, &bytesRead, true);
			CU_ASSERT(success != 0, "RecursiveDirWatch call to GetOverlappedResult failed. Error: '{}', Directory: '{}'", GetLastError(), m_directory);
			CU_ASSERT(bytesRead != 0, "RecursiveDirWatch call to ReadDirectoryChangesW read 0 bytes. Directory: '{}'", m_directory);

			asyncPending = false;

			uint32 cookie = 0;

			FILE_NOTIFY_INFORMATION* fileInformation = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&buffer[0]);
			do {

				std::wstring nameW = std::wstring(fileInformation->FileName, fileInformation->FileNameLength / sizeof(WCHAR));
				std::string name;
				WStringToString(nameW, name);

				fs::path path = name;
				switch (fileInformation->Action) {

				// The wonderful windows file explorer generated Deleted and Created events when a
				// file or directory is moved, or sometimes even when it's renamed. Wonderful!!!!!!
				case FILE_ACTION_REMOVED:
				case FILE_ACTION_ADDED: {

					FileChangeType type = fileInformation->Action == FILE_ACTION_ADDED ? FileChangeType::RenamedNew : FileChangeType::RenamedOld;
					FileChangeType otherType = fileInformation->Action == FILE_ACTION_ADDED ? FileChangeType::RenamedOld : FileChangeType::RenamedNew;

					// Add RenameInfo event if no match was found
					// TODO: Replace the key with inode or some unique identifier

					const auto it = renameMap.find(path.filename().string());
					if (it == renameMap.end()) {

						renameMap.emplace(std::piecewise_construct, std::forward_as_tuple(path.filename().string()), std::forward_as_tuple(path, type, random(), Clock::now()));
						break;

					}

					// Resolve RenameInfo

					RenameInfo& info = it->second;
					CU_ASSERT(info.type == otherType, "RecursiveDirWatch received two rename events with the same type. Directory: '{}', path: '{}', other path: '{}'", m_directory, path, info.path);

					parsedData.emplace_back(info.path, otherType, info.cookie);
					parsedData.emplace_back(path, type, info.cookie);

					renameMap.erase(it);

					break;

				}

				case FILE_ACTION_MODIFIED: {

					parsedData.emplace_back(path, FileChangeType::Changed, 0);
					break;

				}

				// Windows isn't as goated as linux and doesn't provide a cookie for rename events so
				// we have to create our own one. Thankfully a RenamedNew event is guaranteed to come
				// right after a RenamedOld with a miniscule chance of a Changed event inbetween
				case FILE_ACTION_RENAMED_OLD_NAME: {

					cookie = random();
					parsedData.emplace_back(path, FileChangeType::RenamedOld, cookie);
					
					break;

				}
				case FILE_ACTION_RENAMED_NEW_NAME: {

					parsedData.emplace_back(path, FileChangeType::RenamedNew, cookie);
					break;

				}
				default: break;

				}

				if (fileInformation->NextEntryOffset == 0) break;
				fileInformation = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(fileInformation) + fileInformation->NextEntryOffset);

			} while (m_running);

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

	void WStringToString(const std::wstring& wide, std::string& out) {

		int32 size = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
		out.resize(size - 1);

		WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, &out[0], size, nullptr, nullptr);

	}

}