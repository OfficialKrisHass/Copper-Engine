#include "cupch.h"
#include <CopperECS/Registry.h>

#include "Engine/Components/ScriptComponent.h"
#include "Engine/Components/MeshRenderer.h"

namespace Copper {

	ObjectEvent oce;
	ObjectEvent ode;
	ComponentEvent cae;
	ComponentEvent cre;

	void AddObjectCreatedEventFunc(std::function<bool(const Event&)> func) { oce += func; }
	void AddObjectDestroyedEventFunc(std::function<bool(const Event&)> func) { ode += func; }
	void AddComponentAddedEventFunc(std::function<bool(const Event&)> func) { cae += func; }
	void AddComponentRemovedEventFunc(std::function<bool(const Event&)> func) { cre += func; }

}