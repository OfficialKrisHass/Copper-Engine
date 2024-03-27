#pragma once

#include "Base.h"

#include <vector>

namespace Launcher::Dialogs {

	enum class Result {

		Yes,
		No,

		Ok,
		Retry,
		Cancel

	};

	Result Warning(const std::string& title, const std::string& description);
	Result Error(const std::string& title, const std::string& description);

	std::string OpenFile(const std::string& title, const std::vector<std::string>& filters, const std::string initialDir);

}
