#include "cupch.h"
#include "FileWatch.h"

namespace Copper {

    void FileWatch::Start() {

        CUP_FUNCTION();

        if (m_monitorThread.joinable()) {

            LogError("FileWatch is already running. Path: '{}'", m_path);
            return;

        }

        CU_ASSERT(fs::exists(m_path), "Can't start FileWatch, file doesn't exist. Path: '{}'", m_path);
        CU_ASSERT(fs::is_regular_file(m_path), "FileWatch can only watch files (maybe you meant DirWatch). Path: '{}'", m_path);

        StartBackend();

        m_monitorThread = std::thread([this]() { MonitorChanges(); });
        m_destroy = false;

    }
    void FileWatch::Update() {

        CUP_FUNCTION();

        if (m_destroy == true) {

            Stop();
            return;

        }

        CU_ASSERT(m_callback != nullptr, "No callback was assigned to FileWatch. Path: '{}'", m_path);

        std::vector<FileChangeType> data;
        {
            std::lock_guard<std::mutex> lock = std::lock_guard(m_dataMutex);
            data.swap(m_data);
        }

        for (FileChangeType type : data)
            m_callback(m_path, type);

    }
    void FileWatch::Stop() {

        CUP_FUNCTION();

        if (!m_monitorThread.joinable()) return;

        m_destroy = true;

#ifdef CU_WINDOWS
        SendCloseEvent();
#endif
        m_monitorThread.join();

        // To ensure all of the events were reported

        CU_ASSERT(m_callback != nullptr, "No callback was assigned to FileWatch. Path: '{}'", m_path);
        for (FileChangeType type : m_data)
            m_callback(m_path, type);

        m_data.clear();

        StopBackend();

        m_destroy = false;

    }

}
