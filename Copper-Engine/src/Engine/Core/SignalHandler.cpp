#include "cupch.h"
#include "SignalHandler.h"

#include <csignal>

namespace Copper::SignalHandler {

	constexpr static int32 CSignalToSignal(Signal signal) {

		switch (signal) {

		case Signal::Abort: return SIGABRT;
		case Signal::Termination: return SIGTERM;
		case Signal::Interrupt: return SIGINT;

		case Signal::Segfault: return SIGSEGV;
		case Signal::IllegalInstruction: return SIGILL;
		case Signal::FloatingPointExc: return SIGFPE;

		}

		LogWarn("Invalid Copper Signal ({})", (uint32) signal);
		return -1;

	}

	void RegisterHandler(Signal cSignal, HandlerFunc handler) {

		signal(CSignalToSignal(cSignal), handler);

	}

}