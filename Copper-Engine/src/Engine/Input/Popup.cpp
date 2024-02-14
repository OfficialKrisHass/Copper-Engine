#include "cupch.h"
#include "Popup.h"

#include <portable-file-dialogs.h>

namespace Copper::Input {

	PopupResult QuestionPopup(const std::string& title, const std::string& description, bool hasCancel) {

		CUP_FUNCTION();

		pfd::message m = pfd::message(title, description, hasCancel ? pfd::choice::yes_no_cancel : pfd::choice::yes_no, pfd::icon::question);

		switch (m.result()) {

		case pfd::button::yes: return PopupResult::Yes;
		case pfd::button::no: return PopupResult::No;
		case pfd::button::cancel: return PopupResult::Cancel;
		default: return PopupResult::Invalid;

		}

	}
	PopupResult InfoPopup(const std::string& title, const std::string& description, bool hasCancel) {

		CUP_FUNCTION();

		pfd::message m = pfd::message(title, description, hasCancel ? pfd::choice::ok_cancel : pfd::choice::ok, pfd::icon::info);

		switch (m.result()) {

		case pfd::button::ok: return PopupResult::Ok;
		case pfd::button::cancel: return PopupResult::Cancel;
		default: return PopupResult::Invalid;

		}

	}
	PopupResult WarningPopup(const std::string& title, const std::string& description, bool hasCancel) {

		CUP_FUNCTION();

		pfd::message m = pfd::message(title, description, hasCancel ? pfd::choice::yes_no_cancel : pfd::choice::yes_no, pfd::icon::warning);

		switch (m.result()) {

		case pfd::button::yes: return PopupResult::Yes;
		case pfd::button::no: return PopupResult::No;
		case pfd::button::cancel: return PopupResult::Cancel;
		default: return PopupResult::Invalid;

		}

	}
	PopupResult ErrorPopup(const std::string& title, const std::string& description) {

		CUP_FUNCTION();

		pfd::message m = pfd::message(title, description, pfd::choice::ok, pfd::icon::error);

		switch (m.result()) {

		case pfd::button::ok: return PopupResult::Ok;
		default: return PopupResult::Invalid;

		}

	}

}