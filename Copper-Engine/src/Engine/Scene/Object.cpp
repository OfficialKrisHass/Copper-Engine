#include "cupch.h"
#include <CopperECS/CopperECS.h>

namespace Copper {

	ScriptComponent* Object::AddScriptComponent(std::string nameSpace, std::string scriptName) {

		return scene->registry.AddScriptComponent(*this, nameSpace, scriptName);

	}

}