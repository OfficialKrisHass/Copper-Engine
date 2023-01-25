#include "ProjectFileWatcher.h"

#include <FileWatch/FileWatch.hpp>

using namespace Copper;

namespace Editor::ProjectFileWatcher {

	struct ProjectFileWatcherData {

		std::filesystem::path directory;
		std::vector<std::string> filters;
		bool running;

		Unique<filewatch::FileWatch<std::string>> fw;

		std::vector<std::function<void(const std::filesystem::path&, const FileChangeType& changeType)>> callbacks;

	};
	ProjectFileWatcherData data;

	static void FileChangeCallback(const std::string& path, const filewatch::Event changeType);

	void Start() {

		if (data.directory.empty()) { LogError("Can't FileWatch an empty Directory!"); return; }

		data.running = true;
		data.fw = CreateUnique<filewatch::FileWatch<std::string>>(data.directory.string(), FileChangeCallback);

	}
	void Stop() {

		data.running = false;
		data.fw.reset();

	}

	static void FileChangeCallback(const std::string& path, const filewatch::Event changeType) {

		if (!data.running) return;

		std::filesystem::path fsPath(path);
		
		//We have to do this the stupid way of holding each filter as a string
		//instead of having a single string that has all of the filters because
		//for some weird F*CKING reason, if I tried to get an index of a character
		//in the filter string, the value would become random when the code enters
		//a loop. I couldn't even change the variable to hold a different value
		bool extensionCorrect = false;
		for (const std::string& filter : data.filters) {

			if (fsPath.extension() == filter) { extensionCorrect = true; break; }

		}
		if (!extensionCorrect) return;

		FileChangeType type;
		switch (changeType) {

			case filewatch::Event::added: type = FileChangeType::Created; break;
			case filewatch::Event::removed: type = FileChangeType::Deleted; break;
			case filewatch::Event::modified: type = FileChangeType::Changed; break;
			case filewatch::Event::renamed_old: type = FileChangeType::RenamedOldName; break;
			case filewatch::Event::renamed_new: type = FileChangeType::RenamedNewName; break;

		}

		for (size_t i = 0; i < data.callbacks.size(); i++) { data.callbacks[i](data.directory.string() + fsPath.string(), type); }

	}

	void SetDirectory(const std::filesystem::path& directory) { data.directory = directory; }
	void AddFilter(const std::string& filter) { data.filters.push_back(filter); }

	void AddFileChangeCallback(std::function<void(const std::filesystem::path&, const FileChangeType& changeType)> func) { data.callbacks.push_back(func); }

}