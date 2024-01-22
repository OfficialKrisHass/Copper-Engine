#pragma once

#include <Copper.h>

#include "Panels/Panel.h"


namespace Editor {

	class ThemeEditor : public Panel {

	public:
		ThemeEditor() : Panel("Theme Editor") {}

		void SaveTheme(const Copper::fs::path& path);
		void LoadTheme(const Copper::fs::path& path);

	private:
		virtual void UI() override;

		void ColorEditor();

		struct ImGuiStyle* style = nullptr;

	};

}