#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Filesystem/FileChangeType.h"

namespace Copper {

    // Watches a directory and calls the assigned callback everytime a change was detected.. Will crash if file does not exist or isn't a directory when started.
    // This will only watch the directory provided, not it's subdirectories. The DirWatch will be stopped when the watched directory is renamed, deleted or moved.
    // Spawns a separate thread for the monitoring, but callback is called from the main thread.
    // To watch subdirectories take a look at RecursiveDirWatch, to watch a single file take a look at FileWatch.
    class DirWatch {

    public:
        typedef std::function<void(const fs::path& path, FileChangeType type)> Callback;

        DirWatch() = default;
        DirWatch(const fs::path& directory) {

            CUP_FUNCTION();

            m_directory = directory;
            Start();

        }
        ~DirWatch() {

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

        // Getters

        inline bool IsRunning() const { return m_monitorThread.joinable(); }

        // Setters

        inline void SetCalback(Callback callback) { m_callback = callback; }

    private:
        struct FileChange {

            fs::path path;
            FileChangeType type;

            FileChange(const fs::path& path, FileChangeType type) : path(path), type(type) {}

        };

        // Core

        fs::path m_directory;
        Callback m_callback;

        std::atomic<bool> m_destroy = false;
        std::thread m_monitorThread;

        // Implementation

#ifdef CU_LINUX
        int32 m_fd = -1;
#elif CU_WINDOWS
        HANDLE m_handle = nullptr;
        HANDLE m_closeEvent = nullptr;

        void SendCloseEvent();
#endif

        // Received data

        std::vector<FileChange> m_data;
        std::mutex m_dataMutex;

        // Implementation in Platform/<Platform>/<Platform>DirWatch.cpp

        void StartBackend();
        void StopBackend();

        // Called by m_monitorThread
        void MonitorChanges();

    };

}
