#pragma once

#include "Engine/Core/Core.h"

#define CauseExceptionInvalid(argument) mono_raise_exception(mono_get_exception_argument_null(argument))
#define CauseException(message, argument)  mono_raise_exception(mono_get_exception_argument(argument, message))

#define CheckValidEntity(eID) if (eID == INVALID_ENTITY_ID || eID >= GetNumOfEntities() || !(*GetEntityFromID(eID))) {\
						      CauseExceptionInvalid("Entity"); return; }
#define CheckValidEntityWithReturn(eID, ret) if (eID == INVALID_ENTITY_ID || eID >= GetNumOfEntities() || !(*GetEntityFromID(eID))) {\
											 CauseExceptionInvalid("Entity"); return ret; }