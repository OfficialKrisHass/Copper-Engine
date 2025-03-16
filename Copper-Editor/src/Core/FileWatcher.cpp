#include "FileWatcher.h"

#include <FileWatch.hpp>

#include <mutex>

using namespace Copper;

namespace Editor::FileWatcher {

#ifdef CU_LINUX
    typedef std::tuple<std::string, FileChangeType, int> FileChange;
#elif CU_WINDOWS
    typedef std::tuple<fs::path, FileChangeType> FileChange;
#endif

    struct FilewatchData {

        fs::path directory;
        std::unique_ptr<filewatch::FileWatch<std::string>> fw = nullptr;

        std::mutex mutex;
        std::vector<FileChange> changes;

        std::vector<Callback> callbacks;

    };
    FilewatchData data;

#ifdef CU_LINUX
    static void FileChangeCallback(const std::string& directory, const std::string& name, const filewatch::Event changeType, int cookie);
#elif CU_WINDOWS
    static void FileChangeCallback(const std::string& directory, const std::string& name, const filewatch::Event changeType);
#endif

    void Start() { Start(data.directory); }
    void Start(const Copper::fs::path &directory) {

        CUP_FUNCTION();

        CU_ASSERT(!directory.empty(), "Can't start FileWatcher on empty directory!");

        if (data.fw != nullptr)
            Stop();

        data.directory = directory;
        data.fw = std::make_unique<filewatch::FileWatch<std::string>>(directory.string(), FileChangeCallback);

#ifdef CU_LINUX
        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(directory)) {

            if (!entry.is_directory()) continue;

            const fs::path& path = entry.path();
            data.fw->AddDirectory(path, fs::relative(path, data.directory));

        }
#endif

    }
    void Stop() {

        CUP_FUNCTION();

        data.fw.reset();

    }

    void PollChanges() {

        CUP_FUNCTION();

        std::lock_guard<std::mutex> lock(data.mutex);

#ifdef CU_LINUX
        std::unordered_map<int, std::string> renameMap;
#endif

        for (const auto& it : data.changes) {

            const fs::path& path = std::get<0>(it);
            const FileChangeType type = std::get<1>(it);

#ifdef CU_LINUX
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
#endif

            Log("Filewatch event {}, name: '{}'", static_cast<uint32>(type), path);

            for (const Callback& callback : data.callbacks)
                callback(path, type);

        }

        data.changes.clear();

    }

    void AddCallback(Callback callback) { data.callbacks.push_back(callback); }

#ifdef CU_LINUX
    void FileChangeCallback(const std::string& directory, const std::string& name, const filewatch::Event changeType, int cookie) {
#elif CU_WINDOWS
    void FileChangeCallback(const std::string& directory, const std::string& name, const filewatch::Event changeType) {
#endif

        CUP_FUNCTION();

        std::lock_guard<std::mutex> lock(data.mutex);

        const fs::path path = fs::path(directory) / name;
#ifdef CU_LINUX
        data.changes.push_back({ path, static_cast<FileChangeType>(changeType), cookie });
#elif CU_WINDOWS
        data.changes.push_back(std::make_pair(path, static_cast<FileChangeType>(changeType)));
#endif

    }

}
