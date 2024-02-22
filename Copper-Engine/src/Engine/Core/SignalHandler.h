#pragma once

#include "Engine/Core/Core.h"

namespace Copper::SignalHandler {

	enum class Signal : uint32 {

		Abort = 0, // Abnormal termination
		Termination = 1, // Normal Termination
		Interrupt = 2, // Recieved Interrupt (OS or Code)

		Segfault = 3, // Segmentation fault e.g. you done messed up kiddo
		IllegalInstruction = 4, // Whoop Whoop, that's the sound of the police
		FloatingPointExc = 5, // Math gone wrong (EMOTIONAL)

	};

	typedef void(*HandlerFunc)(int32);
	void RegisterHandler(Signal signal, HandlerFunc handler);

}