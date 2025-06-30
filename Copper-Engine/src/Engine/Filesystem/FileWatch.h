#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Filesystem/FileChangeType.h"

namespace Copper {

    // Watches a single file and calls the assigned callback everytime a change was detected. Will crash if file does not exist or isn't a file when started.
    // Once the file is renamed or moved, watching will be stopped
    // Spawns a separate thread for the monitoring, but callback is called from the main thread
    // To watch a directory (folder) take a look at DirWatch and RecursiveDirWatch
    class FileWatch {

    public:
        typedef std::function<void(const fs::path& path, FileChangeType type)> Callback;
        
        FileWatch() = default;
        FileWatch(const fs::path& path) : m_path(path) { Start(); } 

        void Start();
        void Update();
        void Stop();

        inline void Start(const fs::path& path) {

            CUP_FUNCTION();

            m_path = path;
            Start();

        }

        inline void SetCallback(Callback callback) { m_callback = callback; }

        inline bool IsRunning() const { return m_running; }

    private:
        fs::path m_path;
        std::atomic<bool> m_running = false;
        std::thread m_monitorThread;
        Callback m_callback;

        int32 m_fd = -1;

        // Received data

        std::vector<FileChangeType> m_data;
        std::mutex m_dataMutex;

        // Backend, implementation in Platform/<Platform>/<Platform>FileWatch.cpp

        void StartBackend();
        void StopBackend();

        // Called by the monitoring thread
        void MonitorChanges();

    };

}
