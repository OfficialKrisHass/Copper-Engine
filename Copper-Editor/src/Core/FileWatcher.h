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

}
