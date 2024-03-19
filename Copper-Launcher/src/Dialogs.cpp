#include "Dialogs.h"

#include <portable-file-dialogs.h>

namespace Launcher::Dialogs {

	Result Convert(pfd::button button) {

		switch (button) {

		case pfd::button::yes: return Result::Yes;
		case pfd::button::no: return Result::No;
		case pfd::button::ok: return Result::Ok;
		case pfd::button::retry: return Result::Retry;
		case pfd::button::cancel: return Result::Cancel;

		}

		return (Result) 0;

	}

	Result Warning(const std::string& title, const std::string& description) {

		return Convert(pfd::message(title, description, pfd::choice::yes_no, pfd::icon::warning).result());
	
	}
	Result Error(const std::string& title, const std::string& description) {

		return Convert(pfd::message(title, description, pfd::choice::ok, pfd::icon::error).result());

	}

	std::string OpenFile(const std::string& title, const std::vector<std::string>& filters, const std::string initialDir) {

		std::vector<std::string> ret = pfd::open_file(title, initialDir, filters).result();
		return ret.size() != 0 ? ret[0] : "";

	}

}