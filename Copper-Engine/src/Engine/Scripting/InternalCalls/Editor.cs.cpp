#include "cupch.h"
#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/ScriptingCore.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::InternalCalls {

	void EditorLog(MonoString* msg) {

		Log(MonoUtils::MonoToString(msg));

	}
	void EditorLogWarn(MonoString* msg) {

		LogWarn(MonoUtils::MonoToString(msg));

	}
	void EditorLogError(MonoString* msg) {

		LogError(MonoUtils::MonoToString(msg));

	}

}