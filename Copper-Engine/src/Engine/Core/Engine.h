#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Events/Event.h"

namespace Copper {

	void COPPER_API Initialize();
	void COPPER_API Run();
	void COPPER_API Shutdown();

	bool COPPER_API OnWindowResize(Event& e);
	bool COPPER_API OnWindowClose(Event& e);

}