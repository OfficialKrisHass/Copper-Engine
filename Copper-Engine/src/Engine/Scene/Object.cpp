#include "cupch.h"

#include "CopperECS/Object.h"
#include "CopperECS/Scene.h"

namespace Copper {

	void Object::SetName(std::string name) {

		scene->registry.SetObjectName(*this, name);

	}

}