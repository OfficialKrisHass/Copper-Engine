#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Core/Engine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::InternalCalls {

	float GetDeltaTime() {

		return DeltaTime();

	}

}