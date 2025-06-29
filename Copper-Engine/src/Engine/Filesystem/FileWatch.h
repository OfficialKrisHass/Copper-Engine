#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Filesystem/FileChangeType.h"

namespace Copper {

    // Stores the sub watches in an tree, but also keeps an unordered map with the key being the watch and the value a pointer to
    // to the tree node, so that we can retrieve the path from the watch, and get the correct structure of the tree and it's subpath

    class FileWatch {

    public:
        typedef std::function<void(const fs::path& path, FileChangeType type)> Callback;

        FileWatch() = default;
        FileWatch(const fs::path& directory, bool recursive = false) : m_directory(directory), m_recursive(recursive) { Start(); }
        ~FileWatch() {

            CUP_FUNCTION();

            Stop();

        }

        inline void Start(const fs::path& directory, bool recursive = false) {

            CUP_FUNCTION();

            m_directory = directory;
            m_recursive = recursive;

            Start();

        }
        void Start();
        void Update();
        void Stop();

        inline void SetCallback(Callback callback) { m_callback = callback; }

    private:
        struct DirectoryWatch {

            fs::path path;
            bool moved = false;

            DirectoryWatch() = default;
            DirectoryWatch(const fs::path& path) : path(path) {}

        };
        struct FileChange {

            fs::path path; // relative to the file watch directory
            FileChangeType type;

            FileChange(const fs::path& path, FileChangeType type) : path(path), type(type) {}

        };

        // Core data
        
        fs::path m_directory;
        bool m_recursive = false;

        std::atomic<bool> m_running = false;
        std::thread m_monitorThread;

        // Implementation data

        int32 m_fd = -1;
#ifdef CU_LINUX
        std::unordered_map<int32, DirectoryWatch> m_watchMap;
#endif
        Callback m_callback = nullptr;

        // Received data

        std::vector<FileChange> m_data;
        std::mutex m_dataMutex;

        // Backend, implementation in Platform/<Platform>/<Platform>FileWatch.cpp

        void StartBackend();
        void StopBackend();

        // This is called from the monitoring thread
        void MonitorDirectory();

#ifdef CU_LINUX
        int32 AddWatch(const fs::path& path, bool moved = false);
        void RemoveWatch(int32 wd);
#endif

    };

}
