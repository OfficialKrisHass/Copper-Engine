#include "cupch.h"
#include "PhysicsEngine.h"

#include <physx/PxPhysicsAPI.h>

class PhysicsErrorCallback : public physx::PxErrorCallback {

public:
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) {
		LogError("[PhysX Error {}]: {} in {} line {}", code, message, file, line); }

};

namespace Copper::Physics {

	struct PhysicsEngineData {

		physx::PxFoundation* foundation;
		physx::PxPhysics* physics;
		physx::PxTolerancesScale tolerance;

		//Handlers
		physx::PxDefaultAllocator allocator;
		physx::PxDefaultCpuDispatcher* cpuDispatcher;
		PhysicsErrorCallback errCallback;

	};

	PhysicsEngineData data;

	void Initialize() {

		data.foundation = PxCreateFoundation(PX_PHYSICS_VERSION, data.allocator, data.errCallback);
		CU_ASSERT(data.foundation, "Failed to Create PhysX Foundation!");

		data.tolerance.length = 100.0f;
		data.tolerance.speed = 981.0f;
		data.physics = PxCreatePhysics(PX_PHYSICS_VERSION, *data.foundation, data.tolerance, true);

		data.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);

	}

	physx::PxScene* CreatePhysicsScene() {

		physx::PxSceneDesc sceneDesc(data.tolerance);

		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.cpuDispatcher = data.cpuDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

		return data.physics->createScene(sceneDesc);

	}

	physx::PxPhysics* GetPhysicsInstance() { return data.physics; }

}