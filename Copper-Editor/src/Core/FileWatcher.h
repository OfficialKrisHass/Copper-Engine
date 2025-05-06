#pragma once

#include <Copper.h>

namespace Editor::FileWatcher {

    enum class FileChangeType : Copper::uint8 {

        Created = 0,
        Deleted,
        Changed,

        RenamedOldName,
        RenamedNewName,

    };

    typedef std::function<void(const Copper::fs::path&, const FileChangeType)> Callback;

    void Start();
    void Start(const Copper::fs::path& directory);
    void PollChanges();
    void Stop();

    void AddCallback(Callback callback);

    constexpr const char* FileChangeToString(FileChangeType type) {

        switch (type) {

            case FileChangeType::Created: return "Created";
            case FileChangeType::Changed: return "Changed";
            case FileChangeType::Deleted: return "Deleted";
            case FileChangeType::RenamedOldName: return "Renamed new";
            case FileChangeType::RenamedNewName: return "Renamed old";

        }

        LogError("Invalid file change type '{}'", static_cast<Copper::uint8>(type));
        return nullptr;

    }

}
