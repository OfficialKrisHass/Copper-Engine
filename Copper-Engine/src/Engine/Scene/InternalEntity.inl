#pragma once

#include "Scene.h"

namespace Copper {

    template<typename T> T* InternalEntity::AddComponent() { return scene->registry.template AddComponent<T>(id); }
    template<typename T> T* InternalEntity::GetComponent() { return scene->registry.template GetComponent<T>(id); }
    template<typename T> bool InternalEntity::HasComponent() { return scene->registry.template HasComponent<T>(id); }
    template<typename T> void InternalEntity::RemoveComponent() { scene->registry.template RemoveComponent<T>(id); }

}