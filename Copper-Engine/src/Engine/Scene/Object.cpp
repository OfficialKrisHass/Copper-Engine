#include "cupch.h"

#include <CopperECS/CopperECS.h>

namespace Copper {

	ScriptComponent* Object::AddScriptComponent(std::string nameSpace, std::string scriptName) {

		return scene->registry.AddScriptComponent(*this, nameSpace, scriptName);

	}
	ScriptComponent* Object::GetScriptComponent(std::string nameSpace, std::string scriptName) {

		return scene->registry.GetScriptComponent(id, nameSpace, scriptName);

	}
	ScriptComponent* Object::GetScriptComponent(int index) const {

		if (scene->registry.scripts.find(id) == scene->registry.scripts.end()) return nullptr;

		return &scene->registry.scripts[id][index];

	}
	bool Object::HasScriptComponent(std::string nameSpace, std::string scriptName) {

		return scene->registry.HasScriptComponent(id, nameSpace, scriptName);

	}

	int Object::GetNumOfScriptComponents() const {

		if (scene->registry.scripts.find(id) == scene->registry.scripts.end()) return -1;

		return scene->registry.scripts[id].size();

	}

}