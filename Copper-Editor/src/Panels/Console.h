#pragma once

#include <Copper.h>

#include "Panels/Panel.h"

namespace Editor {

	class Console : public Panel {

	public:
		Console() : Panel("Console") {}
		
	private:
		virtual void UI() override;

	};

}