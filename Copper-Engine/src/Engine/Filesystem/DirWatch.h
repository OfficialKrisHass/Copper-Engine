#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Filesystem/FileChangeType.h"

namespace Copper {

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

        inline bool IsRunning() const { return m_running; }

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
        std::atomic<bool> m_running = false;
        Callback m_callback;

        std::atomic<bool> m_destroy = false;
        std::thread m_monitorThread;

        // Implementation

        int32 m_fd = -1;

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
