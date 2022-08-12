#include "cupch.h"
#include "Scene.h"

#include "Engine/Scene/Object.h"

namespace Copper {

	int ECS::cCounter = 0;

	Object Scene::CreateObject(std::string name) {

		Object obj(registry.CreateEntity(), this);

		obj.name = name;

		return obj;

	}

	void Scene::DestroyObject(Object obj) {

		registry.DestroyEntitiy(obj.id);

	}

}