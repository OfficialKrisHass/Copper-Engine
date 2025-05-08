#include "FileWatcher.h"

#include <FileWatch.hpp>

#include <mutex>

using namespace Copper;

namespace Editor::FileWatcher {

#ifdef CU_LINUX
    typedef std::pair<FileChangeType, int> FileChange;
#elif CU_WINDOWS
    typedef FileChangeType FileChange;
#endif

    struct FilewatchData {

        fs::path directory;
        std::unique_ptr<filewatch::FileWatch<std::string>> fw = nullptr;

        std::mutex mutex;
        std::unordered_map<fs::path, FileChange> changes;

        std::vector<Callback> callbacks;

    };
    FilewatchData data;

#ifdef CU_LINUX
    static void FileChangeCallback(const std::string& directory, const std::string& name, const filewatch::Event changeType, int cookie);
#elif CU_WINDOWS
    static void FileChangeCallback(const std::string& directory, const std::string& name, const filewatch::Event changeType);
#endif

    void Start() { Start(data.directory); }
    void Start(const Copper::fs::path& directory) {

        CUP_FUNCTION();

        CU_ASSERT(!directory.empty(), "Can't start FileWatcher on empty directory!");
        CU_ASSERT(fs::exists(directory), "{} Does not exist.", directory);

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

#ifdef CU_LOG_STATUS
        if (GetEngineState() == EngineState::PostInitialization)
            LogStatus("\tFilewatcher started at directory '{}'.", directory.string());
        else
#endif
            Log("Filewatcher started at directory '{}'.", directory.string());

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

            const fs::path& path = it.first;

#ifdef CU_LINUX
            const FileChangeType type = it.second.first;
            const int cookie = it.second.second;

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
#elif CU_WINDOWS
            const FileChangeType type = it.second;
#endif

            Log("Filewatch detected change ({}) at {}", FileChangeToString(type), path);

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
        if (data.changes.contains(path)) return;

#ifdef CU_LINUX
        data.changes[path] = { static_cast<FileChangeType>(changeType), cookie };
#elif CU_WINDOWS
        data.changes[path] = static_cast<FileChangeType>(changeType);
#endif

    }

}
