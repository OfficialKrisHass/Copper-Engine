#include "cupch.h"
#include "Engine/Filesystem/DirWatch.h"
#include "Engine/Filesystem/WatchConstants.h"

#include <sys/inotify.h>

#define FILTERS IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM | IN_MOVE_SELF | IN_DELETE_SELF | IN_IGNORED

namespace Copper {

    void DirWatch::StartBackend() {

        CUP_FUNCTION();

        m_fd = inotify_init1(IN_NONBLOCK);
        CU_ASSERT(m_fd >= 0, "Could not initialize inotify during DirWatch initialization. Directory: '{}'", m_directory);

        int32 wd = inotify_add_watch(m_fd, m_directory.string().c_str(), FILTERS);
        CU_ASSERT(wd >= 0, "Could not add inotify watch to DirWatch at directory '{}'.", m_directory);

    }
    void DirWatch::StopBackend() {

        CUP_FUNCTION();

        CU_ASSERT(m_fd >= 0, "Can't close an invalid inotify file descript.");
        close(m_fd);

    }

    void DirWatch::MonitorChanges() {

        char buffer[BUFFER_SIZE];
        std::vector<FileChange> parsedData;

        while (m_destroy == false) {

            int32 length = read(m_fd, buffer, BUFFER_SIZE);
            if (length < 1) {

                if (errno == EAGAIN || errno == EWOULDBLOCK) {

                    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_LENGTH));
                    continue;

                }

                break;

            }

            parsedData.clear();

            uint32 i = 0;
            while (i < length) {

                inotify_event* event = reinterpret_cast<inotify_event*>(&buffer[i]);
                i += sizeof(inotify_event) + event->len;

                fs::path path = event->name;
                FileChangeType type = FileChangeType::None;

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
                else if (event->mask & IN_MOVE_SELF || event->mask & IN_DELETE_SELF || event->mask & IN_IGNORED) {

                    m_destroy = true;
                    return; // This will also shut down the thread

                }

                parsedData.emplace_back(path, type);

            }

            std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
            m_data.insert(m_data.end(), parsedData.begin(), parsedData.end());

        }

    }

}
