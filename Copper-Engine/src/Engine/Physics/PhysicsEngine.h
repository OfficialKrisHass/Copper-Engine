#pragma once

#include "Engine/Core/Core.h"

namespace physx { class PxScene; class PxRigidActor; }

namespace Copper::PhysicsEngine {

    void Initialize();
    void Shutdown();

    void UpdateScene(physx::PxScene* scene);

    physx::PxScene* CreateScene();
    physx::PxRigidActor* CreateRigidActor(Transform* transform, bool staticBody);

}