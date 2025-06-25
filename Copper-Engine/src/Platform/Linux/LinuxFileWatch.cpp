#include "cupch.h"
#include "Engine/Filesystem/FileWatch.h"

#include <sys/inotify.h>

#include <stack>

#define FILTERS IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO

#define BUFFER_SIZE 0x40000

namespace Copper {

    void FileWatch::StartBackend() {

        CUP_FUNCTION();

        m_fd = inotify_init();
        CU_ASSERT(m_fd >= 0, "Could not initialize inotify during FileWatch initialization. Directory: '{}'", m_directory);

        m_rootWatch.watch = AddWatch(m_directory);
        m_rootWatch.name.clear();

        m_watchMap[m_rootWatch.watch] = &m_rootWatch;

        if (!m_recursive) return;

        Log("Adding recursive watches.");
        WatchSubfolders(m_rootWatch, m_directory);

    }
    void FileWatch::StopBackend() {

        CUP_FUNCTION();

        for (const auto& it : m_watchMap)
            inotify_rm_watch(m_fd, it.first);

        close(m_fd);

        m_watchMap.clear();

    }

    void FileWatch::MonitorDirectory() {

        CUP_FUNCTION();

        char* buffer = new char[BUFFER_SIZE];

        while (m_running == true) {

            ssize_t length = read(m_fd, buffer, BUFFER_SIZE);
            if (length < 1) break;

            std::vector<FileChange> parsedData;
            uint32 i = 0;
            while (i < length) {

                inotify_event* event = reinterpret_cast<inotify_event*>(&buffer[i]);
                i += sizeof(inotify_event) + event->len;

                if (event->len == 0) continue;
                CU_ASSERT(m_watchMap.contains(event->wd), "FileWatch received inotify event from watch '{}', but that watch is not loaded in the watch map. Event name: '{}', mask: '{:x}', FileWatch directory: '{}'", event->wd, event->name, event->mask, m_directory);

                DirectoryWatch* watch = m_watchMap.at(event->wd);
                CU_ASSERT(watch != nullptr, "Watch retrieved from watch map for inotify watch '{}' was nullptr. Directory '{}'", watch->watch, m_directory);

                FileChangeType type;
                if (event->mask & IN_CREATE) 
                    type = FileChangeType::Created;
                else if (event->mask & IN_MODIFY)
                    type = FileChangeType::Changed;
                else if (event->mask & IN_DELETE)
                    type = FileChangeType::Deleted;
                else if (event->mask & IN_MOVED_FROM)
                    type = FileChangeType::RenamedOld;
                else if (event->mask & IN_MOVED_TO)
                    type = FileChangeType::RenamedNew;

                if (m_recursive) {

                    fs::path path = GetWatchPath(*watch) / event->name;
                    parsedData.emplace_back(path, type);

                    switch (type) {

                        case FileChangeType::RenamedNew:
                        case FileChangeType::Created: {

                            if (!fs::is_directory(m_directory / path)) break;

                            DirectoryWatch& newWatch = watch->subFolders.emplace_back(event->name);
                            newWatch.watch = AddWatch(m_directory / path);
                            newWatch.parent = watch;

                            CU_ASSERT(!m_watchMap.contains(newWatch.watch), "FileWatch watch map already contains watch '{}'. Path: '{}', Directory: '{}'", newWatch.watch, path, m_directory);
                            m_watchMap[newWatch.watch] = &newWatch;

                            break;

                        }
                        case FileChangeType::RenamedOld:
                        case FileChangeType::Deleted: {

                            Log("Test");

                            DirectoryWatch* tmp = nullptr;
                            uint32 i;
                            for (i = 0; i < watch->subFolders.size(); i++) {

                                if (watch->subFolders[i].name != event->name) continue;
                                
                                tmp = &watch->subFolders[i];
                                break;

                            }
                            if (tmp == nullptr) break;
                            Log("Success");

                            m_watchMap.erase(tmp->watch);
                            watch->subFolders.erase(watch->subFolders.begin() + i);

                            break;

                        }
                        default: break;

                    }

                } else
                    parsedData.emplace_back(event->name, type);

            }

            std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
            std::swap(parsedData, m_data);

        }

    }

    void FileWatch::WatchSubfolders(DirectoryWatch& root, const fs::path& rootPath) {

        CUP_FUNCTION();

        for (const fs::directory_entry& entry : fs::directory_iterator(rootPath)) {

            if (!entry.is_directory()) continue;

            const fs::path& path = entry.path();

            DirectoryWatch& watch = root.subFolders.emplace_back(path.filename().string());
            watch.watch = AddWatch(path);
            watch.parent = &root;

            CU_ASSERT(!m_watchMap.contains(watch.watch), "FileWatch watch map already contains watch '{}'. Path: '{}', Directory: '{}'", watch.watch, fs::relative(path, rootPath), m_directory);
            m_watchMap[watch.watch] = &watch;

            WatchSubfolders(watch, path);

        }

    }
    int32 FileWatch::AddWatch(const fs::path& path) {

        CUP_FUNCTION();

        CU_ASSERT(m_fd >= 0, "Invalid FileWatch inotify file descriptor. Directory: '{}'", m_directory);
        CU_ASSERT(fs::exists(path), "Tried to add FileWatch watch at path '{}', which does not exist. Directory: '{}'", path, m_directory);

        int ret = inotify_add_watch(m_fd, path.string().c_str(), FILTERS);
        CU_ASSERT(ret >= 0, "Could not add FileWatch watch at path '{}'. Directory '{}'", path, m_directory);

        Log("Added watch at '{}' to FileWatch. Directory: '{}'", fs::relative(path, m_directory), m_directory);

        return ret;

    }

}
