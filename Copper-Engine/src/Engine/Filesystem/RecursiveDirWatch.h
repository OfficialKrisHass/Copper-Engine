#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Filesystem/FileChangeType.h"

namespace Copper {

    // Watches a directory recursively (all of it's subdirectories will be watched as well) and calls the assigned callback with the detected changes. Will crash
    // if directory does not exist or isn't a directory at the time Start is called.
    // Spawns a separate thread for the actual monitoring, but callback is called on the main thread.
    // To watch a single file take a look at FileWatch
    // To watch a directory non recursively, take a look at DirWatch
    class RecursiveDirWatch {

    public:
        typedef std::function<void(const fs::path& path, FileChangeType type, uint32 cookie)> Callback;

        RecursiveDirWatch() = default;
        RecursiveDirWatch(const fs::path& directory) : m_directory(directory) { Start(); }
        ~RecursiveDirWatch() {

            CUP_FUNCTION();

            Stop();

        }

        void Start();
        void Update();
        void Stop();

        inline void Start(const fs::path& directory) {

            CUP_FUNCTION();

            m_directory = directory;
            Start();

        }

        inline bool IsRunning() { return m_running; }

        inline void SetCallback(Callback callback) { m_callback = callback; }

    private:
        struct FileChange {

            fs::path path; // relative to the file watch directory
            FileChangeType type;
            uint32 cookie = 0; // Links rename events. 0 For all other events

            FileChange(const fs::path& path, FileChangeType type, uint32 cookie) : path(path), type(type), cookie(cookie) {}

        };

        // Core data
        
        fs::path m_directory;
        std::atomic<bool> m_running = false;
        Callback m_callback = nullptr;

        std::thread m_monitorThread;

        // Implementation data

#ifdef CU_LINUX
        struct Watch {

            fs::path path;
            bool moved = false;

            Watch() = default;
            Watch(const fs::path& path) : path(path) {}

        };

        int32 m_fd = -1;
        std::unordered_map<int32, Watch> m_watchMap;

        int32 AddWatch(const fs::path& path, bool moved = false);
        void RemoveWatch(int32 wd);
#elif CU_WINDOWS
        HANDLE m_handle = nullptr;
        HANDLE m_closeEvent = nullptr;

        void SendCloseEvent();
#endif

        // Received data

        std::vector<FileChange> m_data;
        std::mutex m_dataMutex;

        // Backend, implementation in Platform/<Platform>/<Platform>RecursiveDirWatch.cpp

        void StartBackend();
        void StopBackend();

        // This is called from the monitoring thread
        void MonitorChanges();

    };

}
