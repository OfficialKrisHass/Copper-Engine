#pragma once

#include "cupch.h"

#include "Engine/Scene/Scene.h"

#include <PxPhysicsAPI.h>

namespace Copper::PhysicsEngine {

    using namespace physx;

    class CollisionNotifier : public PxSimulationEventCallback {

    private:
        virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, uint32 nbPairs) {

            CUP_FUNCTION();

        }
        virtual void onTrigger(PxTriggerPair* pairs, uint32 count) {

            CUP_FUNCTION();

        }

        virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
        virtual void onWake(PxActor** , PxU32 ) {}
        virtual void onSleep(PxActor** , PxU32 ){}
        virtual void onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32) {}

    };

}
