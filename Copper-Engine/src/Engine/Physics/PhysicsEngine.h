#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/PhysicsBody.h"

namespace physx {
	
	class PxScene;
	class PxPhysics;

}

namespace Copper::Physics {

	void Initialize();

	physx::PxScene* CreatePhysicsScene();

	physx::PxPhysics* GetPhysicsInstance();

}