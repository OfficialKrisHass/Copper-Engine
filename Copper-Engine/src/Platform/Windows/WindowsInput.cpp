#include "cupch.h"
#include "Engine/Input/Input.h"

#include "Engine/Core/Engine.h"

#include <GLFW/glfw3.h>

namespace Copper {

	bool IsKey(Key key) {

		bool state = glfwGetKey((GLFWwindow*) GetWindow().GetWindowPtr(), key) == GLFW_PRESS ? true : false;

		return state;

	}

}