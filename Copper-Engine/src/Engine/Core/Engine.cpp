#include "Engine.h"
#include "Engine/Debug/Log.h"

#include <iostream>

namespace Copper {

	void Initialize() {

		Logger::Initialize();

		Log("Engine Successfully Initialized!");

		std::cin.get();

	}

}