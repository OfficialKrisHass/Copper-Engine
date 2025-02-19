#include "FileWatcher.h"

#include <FileWatch.hpp>

#include <mutex>

using namespace Copper;

namespace Editor::FileWatcher {
 
    struct FilewatchData {

        fs::path directory;
        std::unique_ptr<filewatch::FileWatch<std::string>> fw = nullptr;

        std::mutex mutex;
        std::vector<std::pair<std::string, FileChangeType>> changes;

        std::vector<Callback> callbacks;

    };
    FilewatchData data;

    static void FileChangeCallback(const std::string& directory, const std::string& name, const filewatch::Event changeType);

    void Start() { Start(data.directory); }
    void Start(const Copper::fs::path &directory) {

        CUP_FUNCTION();

        CU_ASSERT(!directory.empty(), "Can't start FileWatcher on empty directory!");

        if (data.fw != nullptr)
            Stop();

        data.directory = directory;
        data.fw = std::make_unique<filewatch::FileWatch<std::string>>(directory.string(), FileChangeCallback);

    }
    void Stop() {

        CUP_FUNCTION();

        data.fw.reset();

    }

    void PollChanges() {

        CUP_FUNCTION();

        std::lock_guard<std::mutex> lock(data.mutex);

        for (const auto& it : data.changes) {

            Log("FileChange: '{}', type: {}", it.first, static_cast<uint8>(it.second));

            if (fs::is_directory(data.directory / it.first)) {

                if (it.second == FileChangeType::Created)
                    data.fw->AddDirectory(data.directory / it.first, it.first);
                if (it.second == FileChangeType::Deleted)
                    data.fw->RemoveDirectory(it.first);

            }

            for (const Callback& callback : data.callbacks)
                callback(it.first, it.second);

        }

        data.changes.clear();

    }

    void AddCallback(Callback callback) { data.callbacks.push_back(callback); }

    void FileChangeCallback(const std::string& directory, const std::string& name, const filewatch::Event changeType) {

        CUP_FUNCTION();

        std::lock_guard<std::mutex> lock(data.mutex);

        const fs::path path = fs::path(directory) / name;
        data.changes.push_back({ path, static_cast<FileChangeType>(changeType) });

    }

}
