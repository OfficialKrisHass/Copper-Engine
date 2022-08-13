#include "cupch.h"
#include "Scene.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Scene/Object.h"

#include "Engine/Scene/Components/Mesh.h"

namespace Copper {

	int ECS::cCounter = 0;

	Object Scene::CreateObject(std::string name) {

		Object obj(registry.CreateEntity(), this);

		obj.name = registry.AddComponent<Name>(obj.id);
		obj.name->name = name;

		obj.transform = registry.AddComponent<Transform>(obj.id);

		return obj;

	}

	void Scene::DestroyObject(Object obj) {

		registry.DestroyEntitiy(obj.id);

	}

	void Scene::Update() {

		Renderer::ClearColor(0.18f, 0.18f, 0.18f);

		for (Object o : SceneView<Mesh>(*this)) {

			Renderer::Render(o.GetComponent<Mesh>());

		}

	}

}