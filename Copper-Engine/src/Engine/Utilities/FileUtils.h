#pragma once

#include "Engine/Core/Core.h"

#include "cupch.h"

namespace Copper::Utilities {

	std::string ReadFile(std::string path, uint32_t* size = nullptr);

	std::string SaveDialog(const char* filter);
	std::string OpenDialog(const char* filter);

}