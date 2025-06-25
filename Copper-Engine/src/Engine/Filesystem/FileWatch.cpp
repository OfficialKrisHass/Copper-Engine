#include "cupch.h"
#include "FileWatch.h"

#include <stack>

namespace Copper {

    void FileWatch::Start() {

        CUP_FUNCTION();

        if (m_running == true) {

            LogWarn("FileWatch at directory '{}' already started.", m_directory);
            return;

        }

        Log("Starting FileWatch at directory '{}'. Recursive: '{}'", m_directory, m_recursive);

        CU_ASSERT(fs::exists(m_directory), "Cant't start FileWatch, directory '{}' does not exist!", m_directory);
        StartBackend();

        m_monitorThread = std::thread([this]() { MonitorDirectory(); });
        m_running = true;

    }
    void FileWatch::Update() {

        CUP_FUNCTION();

        if (m_running == false) return;

        CU_ASSERT(m_callback != nullptr, "No callback was assigned to FileWatch at directory '{}'", m_directory);

        std::vector<FileChange> changes;
        {
            std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
            if (m_data.empty()) return;

            std::swap(m_data, changes);
        }

        for (const FileChange& change : changes) {

            Log("FileWatch detected change at '{}', type: '{}' ({})", change.path, FileChangeTypeToString(change.type), static_cast<uint8>(change.type));
            m_callback(change.path, change.type);

        }

    }
    void FileWatch::Stop() {

        CUP_FUNCTION();

        m_running = false;

        StopBackend();

        m_rootWatch.watch = -1;
        m_rootWatch.subFolders.clear();

        {
            std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
            m_data.clear();
        }

        m_monitorThread.join();

    }

    fs::path FileWatch::GetWatchPath(const DirectoryWatch& watch) {

        CUP_FUNCTION();

        const DirectoryWatch* tmp = &watch;
        std::stack<const std::string*> pathParts;
        while (tmp != &m_rootWatch) {

            pathParts.push(&tmp->name);

            CU_ASSERT(tmp->parent != nullptr, "DirectoryWatch '{}' has no parent DirectoryWatch assigned!", tmp->name);
            tmp = tmp->parent;

        }
        fs::path path;
        while (!pathParts.empty()) {

            path /= *pathParts.top();
            pathParts.pop();

        }

        return path;

    }

}
