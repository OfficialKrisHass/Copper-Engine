#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Utils {

	struct TemplateArgument {

		std::string name = nullptr; // :{<name>} in template file
		std::string value = nullptr; // :{<name>} will be replaced with <value>

	};

	void FileFromTemplate(const std::string& templateName, const std::string& outDir, const std::initializer_list<TemplateArgument>& templateArguments);

}