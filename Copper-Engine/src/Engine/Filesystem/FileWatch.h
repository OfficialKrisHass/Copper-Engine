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

            int32 watch = -1;
            std::string name;

            DirectoryWatch* parent = nullptr;
            std::vector<DirectoryWatch> subFolders;

            DirectoryWatch() = default;
            DirectoryWatch(const std::string& name) : name(name) {}

        };
        struct FileChange {

            fs::path path; // relative to the file watch directory
            FileChangeType type;

            FileChange(const fs::path& path, FileChangeType type) : path(path), type(type) {}

        };

        fs::path m_directory;
        bool m_recursive = false;
        std::atomic<bool> m_running = false;

        int32 m_fd = -1;
        DirectoryWatch m_rootWatch;
        Callback m_callback = nullptr;

        std::vector<FileChange> m_data;
        std::mutex m_dataMutex;

        std::thread m_monitorThread;

#ifdef CU_LINUX
        std::unordered_map<int32, DirectoryWatch*> m_watchMap;
#endif

        fs::path GetWatchPath(const DirectoryWatch& watch);

        void StartBackend();
        void StopBackend();

        // This is called from the monitoring thread
        void MonitorDirectory();

#ifdef CU_LINUX
        void WatchSubfolders(DirectoryWatch& root, const fs::path& path);
        int32 AddWatch(const fs::path& path);
#endif

    };

}
