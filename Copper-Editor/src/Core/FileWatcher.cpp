#include "FileWatcher.h"

#include <FileWatch.hpp>

#include <mutex>

using namespace Copper;

namespace Editor::FileWatcher {
 
    struct FilewatchData {

        fs::path directory;
        std::unique_ptr<filewatch::FileWatch<std::string>> fw = nullptr;

        std::mutex mutex;
        std::vector<std::tuple<std::string, FileChangeType, int>> changes;

        std::vector<Callback> callbacks;

    };
    FilewatchData data;

    static void FileChangeCallback(const std::string& directory, const std::string& name, const filewatch::Event changeType, int cookie);

    void Start() { Start(data.directory); }
    void Start(const Copper::fs::path &directory) {

        CUP_FUNCTION();

        CU_ASSERT(!directory.empty(), "Can't start FileWatcher on empty directory!");

        if (data.fw != nullptr)
            Stop();

        data.directory = directory;
        data.fw = std::make_unique<filewatch::FileWatch<std::string>>(directory.string(), FileChangeCallback);

        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(directory)) {

            if (!entry.is_directory()) continue;

            const fs::path& path = entry.path();
            data.fw->AddDirectory(path, fs::relative(path, data.directory));

        }

    }
    void Stop() {

        CUP_FUNCTION();

        data.fw.reset();

    }

    void PollChanges() {

        CUP_FUNCTION();

        std::lock_guard<std::mutex> lock(data.mutex);

        std::unordered_map<int, std::string> renameMap;

        for (const auto& it : data.changes) {

            const std::string& path = std::get<0>(it);
            const FileChangeType type = std::get<1>(it);
            const int cookie = std::get<2>(it);

            if (type == FileChangeType::RenamedOldName)
                renameMap[cookie] = path;
            
            if (fs::is_directory(data.directory / path)) {

                if (type == FileChangeType::Created)
                    data.fw->AddDirectory(data.directory / path, path);
                else if (type == FileChangeType::Deleted)
                    data.fw->RemoveDirectory(path);
                else if (type == FileChangeType::RenamedNewName)
                    data.fw->UpdateDirectory(renameMap[cookie], path);

            }

            Log("Filewatch event {}, name: '{}'", static_cast<uint32>(type), path);

            for (const Callback& callback : data.callbacks)
                callback(path, type);

        }

        data.changes.clear();

    }

    void AddCallback(Callback callback) { data.callbacks.push_back(callback); }

    void FileChangeCallback(const std::string& directory, const std::string& name, const filewatch::Event changeType, int cookie) {

        CUP_FUNCTION();

        std::lock_guard<std::mutex> lock(data.mutex);

        const fs::path path = fs::path(directory) / name;
        data.changes.push_back({ path, static_cast<FileChangeType>(changeType), cookie });

    }

}
