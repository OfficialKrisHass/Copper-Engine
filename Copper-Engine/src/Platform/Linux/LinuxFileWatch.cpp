#include "cupch.h"
#include "Engine/Filesystem/FileWatch.h"

#include <sys/inotify.h>

#define FILTERS IN_CREATE | IN_MODIFY | IN_DELETE_SELF | IN_MOVE_SELF

#define BUFFER_SIZE 0x40000

#define SLEEP_LENGTH 100

namespace Copper {

    void FileWatch::StartBackend() {

        CUP_FUNCTION();

        m_fd = inotify_init1(IN_NONBLOCK);
        CU_ASSERT(m_fd >= 0, "Could not initialize inotify during FileWatch initialization. Path: '{}'", m_path);

        int32 wd = inotify_add_watch(m_fd, m_path.string().c_str(), FILTERS);
        CU_ASSERT(wd >= 0, "Could not add inotify watch to FileWatch. Path: '{}'", m_path);

    }
    void FileWatch::StopBackend() {

        CUP_FUNCTION();

        close(m_fd);

    }

    void FileWatch::MonitorChanges() {

        char* buffer = new char[BUFFER_SIZE];
        while (m_running = true) {

            ssize_t length = read(m_fd, buffer, BUFFER_SIZE);
            if (length < 1) {

                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_LENGTH));
                else break;

                continue;

            }

            std::vector<FileChangeType> parsedData;
            uint32 i = 0;
            while (i < length) {

                inotify_event* event = reinterpret_cast<inotify_event*>(&buffer[i]);
                i += sizeof(inotify_event) + event->len;

                FileChangeType type = FileChangeType::None;
                if (event->mask & IN_CREATE)
                    type = FileChangeType::Created;
                else if (event->mask & IN_MODIFY)
                    type = FileChangeType::Changed;
                else if (event->mask & IN_DELETE_SELF || event->mask & IN_IGNORED) {

                    type = FileChangeType::Deleted;

                    m_destroy = true;
                    return;

                }
                else if (event->mask & IN_MOVE_SELF) {

                    type = FileChangeType::RenamedNew;

                    m_destroy = true;
                    return;

                }

                parsedData.emplace_back(type);

            }

            std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
            m_data.insert(m_data.end(), parsedData.begin(), parsedData.end());

        }

    }

}
