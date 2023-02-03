#pragma once

#include "Copper.h"

#include <ImGui/imgui.h>

namespace Editor {

	class Panel {

	public:
		Panel() = default;
		Panel(const std::string& name) : name(name) {}

		std::string name;

		void UIRender() {

			ImGui::Begin(name.c_str());
			UI();
			ImGui::End();

		}

	private:
		virtual void UI() = 0;

	};

}