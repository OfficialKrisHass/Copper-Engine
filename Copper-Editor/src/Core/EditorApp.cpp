#include "EditorApp.h"

#include <ImGui/imgui.h>

namespace Editor {

	void Initialize() {

		Log("--------------------Editor Initialization");
		Log("Editor Initialization Started!");

		Log("Editor Succesfully Initialized");
		Log("--------------------Editor Initialization\n");

	}

	void Run() {

		//

	}

	void UI() {

		ImGui::Begin("Viewport");
		ImGui::Text("Hello World!");
		ImGui::End();

	}

}