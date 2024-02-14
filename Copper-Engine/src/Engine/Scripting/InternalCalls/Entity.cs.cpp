#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/ScriptingCore.h"

#include "Engine/Scene/CopperECS.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::InternalCalls {

	MonoString* GetEntityName(uint32 eID) {

		CUP_FUNCTION();

		CheckValidEntityWithReturn(eID, nullptr);
		InternalEntity* entity = GetEntityFromID(eID);

		return MonoUtils::StringToMono(entity->name);

	}
	void SetEntityName(uint32 eID, MonoString* out) {

		CUP_FUNCTION();

		CheckValidEntity(eID);
		InternalEntity* entity = GetEntityFromID(eID);

		entity->name = MonoUtils::MonoToString(out);

	}

	bool IsEntityValid(uint32 eID) {

		CUP_FUNCTION();

		if (eID == INVALID_ENTITY_ID || eID >= GetNumOfEntities() || !(*GetEntityFromID(eID))) return false;
		return true;

	}

	MonoObject* GetEntity(uint32 eID) {

		CUP_FUNCTION();

		CheckValidEntityWithReturn(eID, nullptr);

		MonoObject* ret = mono_object_new(Scripting::GetAppDomain(), Scripting::GetEntityMonoClass());
		mono_runtime_object_init(ret);

		MonoClassField* eIDField = mono_class_get_field_from_name(Scripting::GetEntityMonoClass(), "id");
		mono_field_set_value(ret, eIDField, &eID);

		return ret;

	}

}