#include "cupch.h"
#include "DirWatch.h"

namespace Copper {

    void DirWatch::Start() {

        CUP_FUNCTION();

        if (m_running == true) {

            LogWarn("DirWatch is already running. Directory: '{}'", m_directory);
            return;

        }

        CU_ASSERT(fs::exists(m_directory), "Can't start DirWatch at '{}', path does not exist", m_directory);
        CU_ASSERT(fs::is_directory(m_directory), "DirWatch can only watch directories. Directory: '{}'", m_directory);

        StartBackend();

        m_monitorThread = std::thread([this]() { MonitorChanges(); });
        m_running = true;

    }
    void DirWatch::Update() {

        CUP_FUNCTION();

        if (m_destroy == true) {

            Stop();
            return;

        }

        if (m_running == false) return;

        CU_ASSERT(m_callback != nullptr, "No callback was assigned to DirWatch at directory '{}'", m_directory);

        std::vector<FileChange> data;
        {
            std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
            data.swap(m_data);
        }

        for (const FileChange& change : data)
            m_callback(change.path, change.type);

    }
    void DirWatch::Stop() {

        m_running = false;
        m_destroy = false;

        if (m_monitorThread.joinable())
            m_monitorThread.join();
        m_data.clear();

        StopBackend();

    }

}
