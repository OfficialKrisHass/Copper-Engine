#include <cupch.h>
#include "FileTemplate.h"

#include <fstream>

namespace Copper::Utils {

	static void ReplaceArgument(std::string& line, size_t start, const std::string templateName, const std::unordered_map<std::string, std::string>& replacementMap);

	void FileFromTemplate(const std::string& templateName, const std::string& outDir, const std::initializer_list<TemplateArgument>& templateArguments) {

		CUP_FUNCTION();

		// Prepare template arguments upfront

		std::unordered_map<std::string, std::string> replacementMap;
		for (const TemplateArgument& argument : templateArguments)
			replacementMap[argument.name] = argument.value;

		// Fstream setup

		std::ifstream templ;
		std::ofstream out;

		templ.open("assets/Templates/" + templateName + ".cut");
		out.open(outDir);

		// Fill in the file

		std::string line;
		while (std::getline(templ, line)) {

			size_t pos = line.find(":{");
			if (pos != std::string::npos)
				ReplaceArgument(line, pos + 2, templateName, replacementMap);

			out << line + '\n';

		}

		// Cleanup

		templ.close();
		out.close();

	}

	void ReplaceArgument(std::string& line, size_t start, const std::string templateName, const std::unordered_map<std::string, std::string>& replacementMap) {

		size_t end = line.find_first_of('}', start);
		std::string arg = line.substr(start, end - start);

		if (replacementMap.find(arg) == replacementMap.end()) {

			LogError("Found a template argument with no replacement.\n\tTemplate: {}\n\tTemplate Argument: {}", templateName, arg);
			return;

		}

		line.replace(start - 2, arg.size() + 3, replacementMap.at(arg));

		start = line.find(":{");
		if (start == std::string::npos) return;

		ReplaceArgument(line, start, templateName, replacementMap);

	}

}