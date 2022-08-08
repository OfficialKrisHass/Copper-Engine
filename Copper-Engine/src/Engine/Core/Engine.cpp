#include "cupch.h"
#include "Engine.h"

#include "Engine/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Copper {

	struct EngineData {

		Window* window;

	};

	EngineData data;

	void Initialize() {

		Logger::Initialize();

		Log("--------------------Engine Initialization");
		Log("Engine Initialiation started");

		data.window = new Window(WindowData("Copper Engine", 800, 600));

		Log("Engine Succesfully Initialized");
		Log("--------------------Engine Initialization\n");

	}

	void Run() {

		Log("--------------------Engine Run Loop");
		Log("Engine Entered the Run Loop");

		while (!glfwWindowShouldClose((GLFWwindow*) data.window->windowPtr)) {

			data.window->Update();

		}

		Log("Engine Succesfully Exited the Run Loop");
		Log("--------------------Engine Run Loop\n");

	}

	void Shutdown() {

		Log("--------------------Engine Shutdown");
		Log("Engine Entered the Shutdown");

		data.window->Shutdown();

		Log("Engine Succesfully Shutdown");
		Log("--------------------Engine Shutdown\n");

		std::cin.get();

	}

	bool OnWindowResize(Event& e) {

		Log("{0}", e);

		return true;

	}

	bool OnWindowClose(Event& e) {

		Log("{0}", e);

		return true;

	}

}