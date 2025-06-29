#include "cupch.h"
#include "Engine/Filesystem/FileWatch.h"

#include <sys/inotify.h>

#define FILTERS IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_MOVE_SELF

#define BUFFER_SIZE 0x40000

namespace Copper {

    void FileWatch::StartBackend() {

        CUP_FUNCTION();

        m_fd = inotify_init();
        CU_ASSERT(m_fd >= 0, "Could not initialize inotify during FileWatch initialization. Directory: '{}'", m_directory);

        AddWatch(m_directory, m_recursive);

    }
    void FileWatch::StopBackend() {

        CUP_FUNCTION();

        for (const auto& it : m_watchMap)
            inotify_rm_watch(m_fd, it.first);

        close(m_fd);

        m_watchMap.clear();

    }

    void FileWatch::MonitorDirectory() {

        char* buffer = new char[BUFFER_SIZE];

        while (m_running == true) {

            ssize_t length = read(m_fd, buffer, BUFFER_SIZE);
            if (length < 1) break;

            std::vector<FileChange> parsedData;
            uint32 i = 0;
            while (i < length) {

                inotify_event* event = reinterpret_cast<inotify_event*>(&buffer[i]);
                i += sizeof(inotify_event) + event->len;

                CU_ASSERT(m_watchMap.contains(event->wd), "FileWatch received inotify event from watch that is not register in the watch map. Watch: '{}', Event name: '{}', mask: '{:x}, FileWatch directory: '{}'", event->wd, event->name, event->mask, m_directory);

                DirectoryWatch& watch = m_watchMap.at(event->wd);
                const fs::path path = watch.path / event->name;
                const bool isDirectory = event->mask & IN_ISDIR;

                FileChangeType type = FileChangeType::None;
                if (event->mask & IN_CREATE) {

                    type = FileChangeType::Created;

                    if (isDirectory)
                        AddWatch(m_directory / path);

                } else if (event->mask & IN_MOVED_TO) {

                    type = FileChangeType::RenamedNew;
                    if (isDirectory) {

                        int32 wd = AddWatch(m_directory / path, true);

                        CU_ASSERT(m_watchMap.contains(wd), "Renamed new event tried to update watch '{}' but it does't exist in the watch map. Path: '{}', FileWatch Directory: '{}'", wd, path, m_directory);
                        m_watchMap.at(wd).moved = true;

                    }

                } else if(event->mask & IN_MOVE_SELF) {

                    if (watch.moved) {

                        watch.moved = false;
                        continue;

                    }

                    RemoveWatch(event->wd);
                    if (!m_recursive) continue;

                    const std::string watchPath = watch.path.string();
                    for (const auto& it : m_watchMap) {

                        if (it.second.path.string().compare(0, watchPath.size(), watchPath) != 0) continue;
                        RemoveWatch(it.first);

                    }

                    continue;

                } else if (event->mask & IN_IGNORED) {

                    m_watchMap.erase(event->wd);
                    continue;

                } else if (event->mask & IN_MODIFY)
                    type = FileChangeType::Changed;
                else if (event->mask & IN_DELETE)
                    type = FileChangeType::Deleted;
                else if (event->mask & IN_MOVED_FROM)
                    type = FileChangeType::RenamedOld;

                parsedData.emplace_back(path, type);

            }

            std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
            m_data.insert(m_data.end(), parsedData.begin(), parsedData.end());

        }

    }

    int32 FileWatch::AddWatch(const fs::path& path, bool moved) {

        CU_ASSERT(m_fd >= 0, "Invalid FileWatch inotify file descriptor. Directory: '{}'", m_directory);
        CU_ASSERT(fs::exists(path), "Tried to add FileWatch watch at path '{}', which does not exist. Directory: '{}'", fs::relative(path, m_directory), m_directory);

        int wd = inotify_add_watch(m_fd, path.string().c_str(), FILTERS);
        CU_ASSERT(wd >= 0, "Could not add FileWatch watch at path '{}'. Directory '{}'", fs::relative(path, m_directory), m_directory);

        const fs::path rel = fs::relative(path, m_directory);

        // If path is already an existing watch, inotify_add_watch returns it's wd. In that case, path was most likely a directory that is watched but was
        // moved somewhere else, so we only need to update it's path. If inotify_add_watch does not return an existing wd, it means it's a new directory
        const auto it = m_watchMap.find(wd);
        if (it == m_watchMap.end())
            m_watchMap.emplace(wd, DirectoryWatch(rel));
        else if (rel == it->second.path) return wd;
        else
            it->second.path = rel;

        // If path was moved, we also need to setup watches for all it's sub directories
        if (m_recursive && moved) {

            for (const fs::path& subDir : fs::directory_iterator(path)) {

                if (!fs::is_directory(subDir)) continue;
                AddWatch(subDir, true);

            }

        }

        return wd;

    }
    void FileWatch::RemoveWatch(int32 wd) {

        CU_ASSERT(m_watchMap.contains(wd), "Watch '{}' can not be removed, as it doesn't exist in the watch map. Directory: '{}'", wd, m_directory);

        inotify_rm_watch(m_fd, wd);

    }

}
