#include "Window.h"
#include "UI.h"

#include <ImGui/imgui.h>

#include <iostream>

namespace Launcher {

	struct Data {

		bool running = true;

	};
	static Data data;

	static void Run();
	static void Shutdown();

	int Entry() {

		std::cout << "Hello, World!\n";

		Window::Create(1280, 720, "Copper Launcher");
		UI::Initialize();

		Run();

		return 0;

	}
	void Run() {

		bool boo = false;

		while (data.running) {

			Window::Update();

			UI::BeginFrame();

			ImGui::Text("The quick brown fox jumped over the big brown lazy dog");
			if (ImGui::Button("Jump scare"))
				boo = true;

			if (boo)
				ImGui::Text("Boo");

			UI::EndFrame();

		}

		Shutdown();

	}
	void Shutdown() {

		UI::Shutdown();
		Window::Shutdown();

	}

	void OnWindowClose() {

		data.running = false;

	}

}