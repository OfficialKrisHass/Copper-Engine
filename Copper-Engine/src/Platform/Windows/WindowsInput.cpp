#include "cupch.h"
#include "Engine/Input/Input.h"

#include "Engine/Core/Engine.h"
#include "Engine/Core/Window.h"

#include "Engine/Events/MouseEvent.h"

#include <GLFW/glfw3.h>
#include <portable-file-dialogs.h>

#ifdef CU_EDITOR
extern Copper::UVector2I GetViewportCentre();
#endif

#define GLFW_WINDOW(win) (GLFWwindow*) win->GetWindowPtr()

// TODO: Rework this to make it less complicated

namespace Copper::Input {

	Window* window;

	std::unordered_map<KeyCode, std::pair<uint32, bool>> keys;

	bool mouseVisible = true;
	bool mouseLocked = false;
	bool firstMouseLockedFrame = true;

	UVector2I prevMousePos;
	Vector2 mousePosDiference;

	bool OnKeyPressed(const Event& e);
	bool OnKeyReleased(const Event& e);

	bool OnMouseMove(const Event& e);

	void Initialize(Window& win) {

		CUP_FUNCTION();

		VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize Input");
		window = &win;

		window->AddKeyPressedEventFunc(OnKeyPressed);
		window->AddKeyReleasedEventFunc(OnKeyReleased);

		window->AddMouseMoveEventFunc(OnMouseMove);

		if (!pfd::settings::available())
		#ifdef CU_LINUX
			LogError("Portable File Dialogs are not available! You might be missing these packages:\n\tKDE: KDialog\n\tGnome: Zenity/Matedialog/Qarma");
		#elif CU_WINDOWS
			LogError("Portable File Dialogs are not available!");
		#endif

		pfd::settings::verbose(true);

	}
	void Update() {

		CUP_FUNCTION();

		mousePosDiference = Vector2::zero;

	}

	bool IsKey(KeyCode key) {

		CUP_FUNCTION();

		if (keys[key].first == 0) return false;

		return true;

	}
	bool IsKeyDown(KeyCode key) {

		CUP_FUNCTION();

		if (keys[key].first != 1) return false;

		keys[key].first++;
		return true;

	}
	bool IsKeyReleased(KeyCode key) {

		CUP_FUNCTION();

		if (!keys[key].second) return false;

		keys[key].second = false;
		return true;

	}

	bool IsButton(MouseCode button) {

		CUP_FUNCTION();

		return glfwGetMouseButton(GLFW_WINDOW(window), (int32) button) == GLFW_PRESS ? true : false;

	}

	bool OnKeyPressed(const Event& e) {

		CUP_FUNCTION();

		KeyCode keycode = ((KeyEvent*)&e)->key;
		keys[keycode].first++;
		keys[keycode].second = false;

		return true;

	}
	bool OnKeyReleased(const Event& e) {

		CUP_FUNCTION();

		KeyCode keycode = ((KeyEvent*)&e)->key;
		keys[keycode].first = 0;
		keys[keycode].second = true;

		return true;

	}

	bool OnMouseMove(const Event& e) {

		CUP_FUNCTION();

		MouseMoveEvent& event = *((MouseMoveEvent*)&e);

		if (!mouseLocked) {

			UVector2I diferenceFull = prevMousePos - event.mouseCoords;
			mousePosDiference.x = -((float) diferenceFull.x / (float) GetWindow().Width());
			mousePosDiference.y = -((float) diferenceFull.y / (float) GetWindow().Height());

			prevMousePos = event.mouseCoords;

			return true;

		}

		UVector2I centre;
#ifdef CU_EDITOR
		centre = GetViewportCentre();
#else
		centre = GetWindowSize() / 2;
#endif

		if (event.mouseCoords == centre) return true;
		if (!firstMouseLockedFrame) {

			mousePosDiference.x = ((float) event.mouseCoords.x - centre.x) / GetWindowSize().x;
			mousePosDiference.y = ((float) event.mouseCoords.y - centre.y) / GetWindowSize().y;

		}
		else {

			firstMouseLockedFrame = false;

		}

		prevMousePos = event.mouseCoords;

		SetCursorPosition((float) centre.x, (float) centre.y);

		return true;

	}

	void SetCursorVisible(bool visible) {

		CUP_FUNCTION();

		glfwSetInputMode(GLFW_WINDOW(window), GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
		mouseVisible = visible;

	}
	void SetCursorLocked(bool locked) {

		CUP_FUNCTION();

		mouseLocked = locked;
		if (locked) firstMouseLockedFrame = true;

	}
	void SetCursorPosition(float x, float y) {

		CUP_FUNCTION();

		glfwSetCursorPos(GLFW_WINDOW(window), x, y);

	}

	void SetWindowTitle(const std::string& title) {

		CUP_FUNCTION();

		glfwSetWindowTitle(GLFW_WINDOW(window), title.c_str());

	}

	void GetCursorPosition(double* x, double* y) {

		CUP_FUNCTION();

		glfwGetCursorPos(GLFW_WINDOW(window), x, y);

	}

	float GetCursorPosDifferenceX() { return mousePosDiference.x; }
	float GetCursorPosDifferenceY() { return mousePosDiference.y; }

	bool IsCursorLocked() {

		return mouseLocked;

	}

	bool IsCursorVisible() {

		return mouseVisible;

	}

}