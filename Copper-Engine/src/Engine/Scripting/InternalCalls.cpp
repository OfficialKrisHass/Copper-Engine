#include "cupch.h"

#include "Engine/Scripting/MonoUtils.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::InternalCalls {

	static void EditorLog(MonoString* msg) { Log(MonoUtils::MonoToString(msg)); }
	static void EditorLogWarn(MonoString* msg) { LogWarn(MonoUtils::MonoToString(msg)); }
	static void EditorLogError(MonoString* msg) { LogError(MonoUtils::MonoToString(msg)); }

}