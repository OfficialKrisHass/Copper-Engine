#pragma once

#include <Copper.h>

#include "Panels/Panel.h"


namespace Editor {

	class ThemeEditor : public Panel {

	public:
		ThemeEditor() : Panel("Theme Editor") {}

		void SaveTheme(const fs::path& path);
		void LoadTheme(const fs::path& path);

	private:
		virtual void UI() override;

		void ColorEditor();

		struct ImGuiStyle* style = nullptr;

	};

}