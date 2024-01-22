#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Core/Engine.h"

#include "Engine/Input/Input.h"
#include "Engine/Input/AxisManager.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::InternalCalls {

	bool IsKey(int32 keyCode) {

	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return false;
	#endif
		return Input::IsKey((KeyCode) keyCode);

	}
	bool IsKeyDown(int32 keyCode) {

	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return false;
	#endif
		return Input::IsKeyDown((KeyCode) keyCode);

	}
	bool IsKeyReleased(int32 keyCode) {

	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return false;
	#endif
		return Input::IsKeyReleased((KeyCode) keyCode);

	}

	float GetAxis(MonoString* axisName) {

	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return 0.0f;
	#endif
		return Input::GetAxis(MonoUtils::MonoToString(axisName));

	}

	void SetCursorVisible(bool visible) {

	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return;
	#endif
		Input::SetCursorVisible(visible);

	}
	void SetCursorLocked(bool locked) {

	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return;
	#endif
		Input::SetCursorLocked(locked);

	}

}