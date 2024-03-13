#include "FileWatcher.h"

#include <FileWatch/FileWatch.hpp>

using namespace Copper;

namespace Editor::FileWatcher {

	constexpr FileChangeType FWToCopper(const filewatch::Event value) {

		switch (value) {

		case filewatch::Event::added: return FileChangeType::Created;
		case filewatch::Event::removed: return FileChangeType::Deleted;
		case filewatch::Event::modified: return FileChangeType::Changed;
		case filewatch::Event::renamed_old: return FileChangeType::RenamedOldName;
		case filewatch::Event::renamed_new: return FileChangeType::RenamedNewName;

		}

		LogError("Invalid fileWatch::Event value!");
		return (FileChangeType) -1;

	}

	typedef std::pair<fs::path, FileChangeType> FileChange;

	struct FileWatcherData {

		fs::path directory;
		std::unique_ptr<filewatch::FileWatch<std::string>> fw;

		std::vector<Callback> callbacks;

		bool polling = false;
		std::vector<FileChange> fileChanges;

	};
	FileWatcherData data;

	static void FileChangeCallback(const std::string& path, const filewatch::Event changeType);

	void Start() { Start(data.directory); }
	void Start(const fs::path& directory) {

		if (directory.empty()) { LogError("Can't FileWatch an empty Directory!"); return; }

		data.directory = directory;
		data.fw = std::make_unique<filewatch::FileWatch<std::string>>(directory.string(), FileChangeCallback);

	}
	void PollCallbacks() {

		data.polling = true;

		for (const FileChange& event : data.fileChanges) {

			for (Callback& callback : data.callbacks)
				callback(event.first, event.second);

		}
		data.fileChanges.clear();

		data.polling = false;

	}
	void Stop() {

		data.fw.reset();

	}

	void SetDirectory(const fs::path& directory) { data.directory = directory; }

	void AddCallback(Callback callback) { data.callbacks.push_back(callback); }

	static void FileChangeCallback(const std::string& path, const filewatch::Event changeType) {

		while (data.polling) {}
		data.fileChanges.push_back({ data.directory / path, FWToCopper(changeType) });

	}

}