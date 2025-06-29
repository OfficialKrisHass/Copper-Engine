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

        std::vector<FileChange> data;
        {
            std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
            data.swap(m_data);
        }

        for (const FileChange& change : data) {

            m_callback(change.path, change.type);

        }

    }
    void FileWatch::Stop() {

        CUP_FUNCTION();

        m_running = false;

        StopBackend();

        m_monitorThread.join();
        {
            std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
            m_data.clear();
        }

    }

}
