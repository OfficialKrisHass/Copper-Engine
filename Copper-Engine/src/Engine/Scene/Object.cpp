#include "cupch.h"

#include <CopperECS/CopperECS.h>

namespace Copper {

	ScriptComponent* Object::AddScriptComponent(std::string nameSpace, std::string scriptName) {

		return scene->registry.AddScriptComponent(*this, nameSpace, scriptName);

	}

	int Object::GetNumOfScriptComponents() const {

		if (scene->registry.scripts.find(id) == scene->registry.scripts.end()) return -1;

		return scene->registry.scripts[id].size();

	}

	ScriptComponent* Object::GetScriptComponent(int index) const {

		if (scene->registry.scripts.find(id) == scene->registry.scripts.end()) return nullptr;

		return &scene->registry.scripts[id][index];

	}

}